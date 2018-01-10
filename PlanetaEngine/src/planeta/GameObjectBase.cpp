#include <cassert>
#include "boost/algorithm/string.hpp"

#include "Game.hpp"
#include "GameObjectBase.hpp"
#include "GameObjectComponent.hpp"
#include "GameObjectComponentSetUpData.hpp"
#include "planeta/LogUtility.hpp"
#include "Task.hpp"
#include "GOComponentAdder.hpp"
#include "GOComponentGetter.hpp"
#include "RPtree.hpp"
#include "planeta/IResourceManager.hpp"
#include "Reflection.hpp"
#include "planeta/ISceneInternal.hpp"

namespace plnt {
	namespace {
		std::vector<std::shared_ptr<const boost::property_tree::ptree>> GetComDefPtrees(const boost::property_tree::ptree& pt) {
			std::vector<std::shared_ptr<const boost::property_tree::ptree>> out;
			//文字列として読み込んでみる
			std::string res_id = pt.get_value<std::string>();
			//文字列が空でなかったらリソースIDとみなしてPtreeリソースを読み込む
			if (!res_id.empty()) {
				auto jres = Game::instance().resource_manager()->GetResourceByID<RPtree>(res_id);
				//指定されたリソースがない
				if (jres == nullptr) {
					PE_LOG_ERROR("ゲームオブジェクトコンポーネントのファイル定義読み込みに失敗しました。指定されたPtreeリソース\"", res_id, "\"を読み込めませんでした。");
				} else {
					out.push_back(jres->GetPtree());
				}
			} else { //文字列でなかったら、オブジェクトか配列
				if (pt.size() == 0 || !pt.ordered_begin()->first.empty()) { //要素がないか、最初の要素にキーが存在したらオブジェクト。オブジェクトならそれがGOCDefinition
					out.push_back(std::make_shared<const boost::property_tree::ptree>(pt));
				} else { //配列にはGOC定義か、そのリソースIDが含まれる。
					for (auto&& ary_elm : pt) {
						auto res_id2 = ary_elm.second.get_value<std::string>();
						if (!res_id2.empty()) { //要素が文字列だったら、そのIDのリソースを読み込み
							auto jres = Game::instance().resource_manager()->GetResourceByID<RPtree>(res_id2);
							//指定されたリソースがない
							if (jres == nullptr) {
								PE_LOG_ERROR("ゲームオブジェクトコンポーネントのファイル定義読み込みに失敗しました。指定されたPtreeリソース\"", res_id2, "\"を読み込めませんでした。");
							} else {
								out.push_back(jres->GetPtree());
							}
						} else { //そうじゃなかったらそれがGOC定義の一つ
							out.push_back(std::make_shared<const boost::property_tree::ptree>(ary_elm.second));
						}
					}
				}
			}
			return out;
		}
	}

	namespace private_ {
		GameObjectBase::GameObjectBase() {}

		GameObjectBase::~GameObjectBase() = default;

		WeakPointer<IGameObject> GameObjectBase::GetPointer() {
			assert(shared_from_this() != nullptr);
			return shared_from_this();
		}

		void GameObjectBase::Activate() {
			if(!manager_connection_->RequestActivation()) {
				PE_LOG_FATAL("ゲームオブジェクトの有効化に失敗しました。");
			}
		}

		void GameObjectBase::Inactivate() {
			if (!manager_connection_->RequestInactivation()) {
				PE_LOG_FATAL("ゲームオブジェクトの無効化に失敗しました。");
			}
		}

		void GameObjectBase::Dispose() {
			manager_connection_->RequestDisposal();
		}

		bool GameObjectBase::ProcessInitialization() {
			state_ = GameObjectState::Initializing;

			GOComponentGetter com_getter(component_holder_);

			decltype(auto) com_ary = component_holder_.component_array();

			for (auto&& com : com_ary) {
				if (!com->GetOtherComponentsProc(com_getter)) {
					PE_LOG_ERROR("GameObjectComponent\"型:", typeid(*com).name(), "\"でほかのオブジェクトを取得する処理に失敗しました。");
					state_ = GameObjectState::Invalid;
					return false;
				}
			}

			for (auto&& com : com_ary) {
				com->Initialize();
			}
			state_ = GameObjectState::Inactive;
			return true;
		}

		bool GameObjectBase::ProcessActivation() {
			state_ = GameObjectState::Activating;
			//コンポーネントの有効化
			decltype(auto) com_ary = component_holder_.component_array();
			for (auto&& com : com_ary) {
				com->Activate();
			}
			//有効化イベント
			activated();
			//アタッチされたタスクの再開
			CheckAndApplyProcessToAttachedTask([](Task& t)-> bool {return t.Resume(); });

			state_ = GameObjectState::Active;
			return true;
		}

		bool GameObjectBase::ProcessInactivation() {
			state_ = GameObjectState::Inactivating;
			//アタッチされたタスクの停止
			CheckAndApplyProcessToAttachedTask([](Task& t)-> bool {return t.Pause(); });
			//無効化イベント
			inactivated();
			//コンポーネントの無効化
			decltype(auto) com_ary = component_holder_.component_array();
			for (auto&& com : com_ary) {
				com->InActivate();
			}
			
			state_ = GameObjectState::Inactive;
			return true;
		}

		bool GameObjectBase::ProcessDisposal() {
			state_ = GameObjectState::Invalid;
			//アタッチされたタスクの破棄
			CheckAndApplyProcessToAttachedTask([](Task& t)-> bool {t.Dispose(); return true; });
			attached_tasks_.clear();
			//破棄時イベント
			disposed();
			//コンポーネントの終了処理
			for (auto&& com : component_holder_.component_array()) {
				com->Finalize();
			}
			return true;
		}

		void GameObjectBase::SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn) {
			manager_connection_ = std::move(mgr_cnctn);
		}

		void GameObjectBase::SetSceneInternalInterface(const WeakPointer<private_::ISceneInternal>& iscene) {
			scene_internal_interface_ = iscene;
		}

		void GameObjectBase::SetSceneAndGODataToComponent_(GameObjectComponent& com) {
			private_::GameObjectComponentSetUpData rd{ this, scene_internal_interface_ };
			com.SetSceneAndHolderGOData(rd);
		}

		bool GameObjectBase::AddComponentsFromTypeIDList_(const std::vector<std::string>& com_type_id_list, std::vector<std::shared_ptr<GameObjectComponent>>& added_coms) {
			private_::GOComponentAdder com_adder{ component_holder_ };
			for (auto&& com_type_id : com_type_id_list) {
				auto com = com_adder.CreateAndAddComponent(com_type_id);
				if (com) {
					added_coms.push_back(com);
				}else{
					PE_LOG_ERROR("ゲームオブジェクトコンポーネント(GOCTypeID:", com_type_id, ")の追加に失敗しました。");
					return false;
				}
			}
			return true;
		}

		bool GameObjectBase::SetDataToComponentsFromPtree_(const std::vector<std::shared_ptr<GameObjectComponent>>& coms, const std::vector<std::shared_ptr<const boost::property_tree::ptree>>& pts) {
			assert(coms.size() == pts.size());
			for (size_t i = 0; i < coms.size();++i) {
				//Ptreeからリフレクションシステムを用いて読み込み
				try {
					coms[i]->ReflectiveLoadFromPtree(*pts[i]);
				} catch (reflection_error& e) {
					PE_LOG_ERROR("ゲームオブジェクトコンポーネント(\"型:", typeid(*coms[i]).name(), "\")のファイル定義読み込みに失敗しました。エラーが発生したか、コンポーネントがファイル定義読み込みに対応していない可能性があります。ファイル定義読み込み関数を継承しているか確認してください。(", e.what(), ")");
					return false;
				}
			}
			return  true;
		}

		std::shared_ptr<GameObjectComponent> GameObjectBase::GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker) const {
			return component_holder_.GetComponentByTypeInfo(ti, type_checker);
		}

		//std::vector<std::shared_ptr<GameObjectComponent>> GameObjectBase::GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker) const {
		//	return std::move(component_holder_.GetAllComponentsByTypeInfo(ti, type_checker));
		//}

		void GameObjectBase::SetUpAttachedTask_(const WeakPointer<Task>& task) {
			if (state_ == GameObjectState::Active || state_ == GameObjectState::Activating) {
				task->Resume();
			}
			attached_tasks_.push_back(task);
		}

		bool GameObjectBase::ProcessClonation(const std::shared_ptr<GameObjectBase>& dst) {
			//シーンデータのセット
			scene_internal_interface_ = dst->scene_internal_interface_;
			//コンポーネントのクローン
			dst->component_holder_.CloneToOtherHolder(component_holder_);
			//コンポーネントへシーンとGOデータをセット
			SetSceneAndGODataToCOmponents_();
			return true;
		}

		bool GameObjectBase::AddAndSetUpComponents(const boost::property_tree::ptree& pt) {
			//コンポーネント型IDと対応するPtreeの読み込み
			std::vector<std::string> com_type_ids;
			std::vector<std::vector<std::shared_ptr<const boost::property_tree::ptree>>> pts_each_com_tid;
			for (auto&& com_defs : pt) {
				auto com_def_pts = GetComDefPtrees(com_defs.second);
				pts_each_com_tid.push_back(std::move(com_def_pts));
				com_type_ids.push_back(com_defs.first);
			}
			assert(com_type_ids.size() == pts_each_com_tid.size());
			//コンポーネントの追加
			std::vector<std::vector<std::shared_ptr<GameObjectComponent>>> coms_each_com_tid;
			for (size_t i = 0; i < com_type_ids.size(); ++i) {
				std::vector<std::shared_ptr<GameObjectComponent>> coms;
				std::vector<std::string> com_tid_list;
				com_tid_list.resize(pts_each_com_tid[i].size(), com_type_ids[i]);
				if (!AddComponentsFromTypeIDList_(com_tid_list, coms)) {
					PE_LOG_ERROR("コンポーネントの作成に失敗しました。");
					return false;
				}
				coms_each_com_tid.push_back(std::move(coms));
			}
			assert(com_type_ids.size() == coms_each_com_tid.size());
			//コンポーネントのセットアップ
			SetSceneAndGODataToCOmponents_();
			//コンポーネントデータをptreeから読み込み
			for (size_t i = 0; i < com_type_ids.size(); ++i) {
				if (!SetDataToComponentsFromPtree_(coms_each_com_tid[i], pts_each_com_tid[i])) {
					PE_LOG_ERROR("PtreeからのComponentのデータセットに失敗しました。");
					return false;
				}
			}
			return true;
		}

		bool GameObjectBase::AddAndSetUpComponents(const std::vector<std::string>& com_type_id_list) {
			//コンポーネントの追加
			std::vector<std::shared_ptr<GameObjectComponent>> coms;
			if (!AddComponentsFromTypeIDList_(com_type_id_list, coms)) {
				PE_LOG_ERROR("コンポーネントの作成に失敗しました。");
				return false;
			}
			//コンポーネントのセットアップ
			SetSceneAndGODataToCOmponents_();
			return true;
		}

		void GameObjectBase::SetSceneAndGODataToCOmponents_() {
			decltype(auto) com_ary = component_holder_.component_array();
			for (auto&& com : com_ary) {
				SetSceneAndGODataToComponent_(*com);
			}
		}

		bool GameObjectBase::CheckAndApplyProcessToAttachedTask(const std::function<bool(Task&)>& proc) {
			bool scc = true;
			for (auto it = attached_tasks_.begin(); it != attached_tasks_.end();) {
				auto pre = it++;
				if (!(*pre)) {
					attached_tasks_.erase(pre);
					continue;
				} else {
					scc &= proc(**pre);
				}
			}
			return scc;
		}

		GameObjectState GameObjectBase::state()const {
			return state_;
		}

		plnt::IScene& GameObjectBase::scene() {
			return *scene_internal_interface_;
		}

	}
}