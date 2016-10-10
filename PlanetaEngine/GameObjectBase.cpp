#include <cassert>
#include "GameObjectBase.h"
#include "GameObjectManagerPublicInterface.h"
#include "SceneData.h"
#include "GameObjectComponent.h"
#include "GameObjectComponentSetUpData.h"
#include "SystemLog.h"
#include "TGameObjectOperation.h"
#include "GOComponentAdder.h"
#include "GOComponentGetter.h"
#include "RPtree.h"
#include "ResourceManager.h"
#include "Reflection.h"

namespace planeta {
	namespace private_ {
		namespace {
			std::pair<std::string, std::string> SeparateAliasAndComponentTypeID(const std::string str) {
				auto del_pos = str.find_first_of(':');
				if (del_pos == std::string::npos) {
					return{ str,str }; //エイリアスが指定されていないので、型IDをエイリアスとする
				} else {
					auto alias = str.substr(0, del_pos);
					auto tid = str.substr(del_pos + 1, str.size() - del_pos - 1);
					return{ alias,tid };
				}
			}
		}

		GameObjectBase::GameObjectBase() {}

		GameObjectBase::~GameObjectBase() = default;

		WeakPointer<IGameObject> GameObjectBase::GetPointer() {
			assert(shared_from_this() != nullptr);
			return shared_from_this();
		}

		bool GameObjectBase::Activate() {
			return manager_connection_->RequestActivation();
		}

		bool GameObjectBase::Inactivate() {
			return manager_connection_->RequestInactivation();
		}

		void GameObjectBase::Dispose() {
			manager_connection_->RequestDisposal();
		}

		bool GameObjectBase::ProcessInitialization() {
			GOComponentGetter com_getter(component_holder_);

			decltype(auto) com_alias_idx_map = component_holder_.alias_idx_map();
			decltype(auto) com_ary = component_holder_.component_array();

			for (auto&& alias_idx : com_alias_idx_map) {
				if (!com_ary[alias_idx.second]->GetOtherComponentsProc(com_getter)) {
					PE_LOG_ERROR("GameObjectComponent\"エイリアス:", alias_idx.first, "\"でほかのオブジェクトを取得する処理に失敗しました。");
					return false;
				}
			}

			for (auto&& alias_idx : com_alias_idx_map) {
				if (!com_ary[alias_idx.second]->Initialize()) {
					PE_LOG_ERROR("GameObjectComponent\"エイリアス:", alias_idx.first, "\"の初期化処理に失敗しました。");
					return false;
				}
			}
			return true;
		}

		bool GameObjectBase::ProcessActivation() {
			decltype(auto) com_alias_idx_map = component_holder_.alias_idx_map();
			decltype(auto) com_ary = component_holder_.component_array();
			for (auto&& alias_idx : com_alias_idx_map) {
				if (!com_ary[alias_idx.second]->Activate()) {
					PE_LOG_ERROR("GameObjectComponent\"エイリアス:", alias_idx.first, "\"の有効化処理に失敗しました。");
					return false;
				}
			}
			activated_event_delegate_();
			return true;
		}

		bool GameObjectBase::ProcessInactivation() {
			inactivated_event_delegate_();

			decltype(auto) com_alias_idx_map = component_holder_.alias_idx_map();
			decltype(auto) com_ary = component_holder_.component_array();
			for (auto&& alias_idx : com_alias_idx_map) {
				if (!com_ary[alias_idx.second]->InActivate()) {
					PE_LOG_ERROR("GameObjectComponent(\"エイリアス:", alias_idx.first, "\"の無効化処理に失敗しました。");
					return false;
				}
			}
			return true;
		}

		bool GameObjectBase::ProcessDisposal() {
			disposed_event_delegate_();

			for (auto&& com : component_holder_.component_array()) {
				com->Finalize();
			}
			return true;
		}

		void GameObjectBase::SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn) {
			manager_connection_ = std::move(mgr_cnctn);
		}

		void GameObjectBase::SetSceneData(const WeakPointer<private_::SceneData>& scene_accessor) {
			scene_data_ = scene_accessor;
		}

		DelegateConnection GameObjectBase::AddActivatedEventHandler(DelegateHandlerAdder<void>&& hander_adder) {
			return hander_adder(activated_event_delegate_);
		}

		DelegateConnection GameObjectBase::AddInactivatedEventHandler(DelegateHandlerAdder<void>&& hander_adder) {
			return hander_adder(inactivated_event_delegate_);
		}

		DelegateConnection GameObjectBase::AddDisposedEventHandler(DelegateHandlerAdder<void>&& hander_adder) {
			return hander_adder(disposed_event_delegate_);
		}

		void GameObjectBase::SetSceneAndGODataToComponent_(GameObjectComponent& com) {
			private_::GameObjectComponentSetUpData rd{ this, scene_data_ };
			com.SetSceneAndHolderGOData(rd);
		}

		bool GameObjectBase::AddComponentsFromTypeIDList_(const std::vector<std::string>& com_type_id_list) {
			private_::GOComponentAdder com_adder{ component_holder_ };
			for (auto&& com_type_id : com_type_id_list) {
				auto alias_tid = SeparateAliasAndComponentTypeID(com_type_id);
				if (!com_adder.CreateAndAddComponent(alias_tid.first, alias_tid.second)) {
					PE_LOG_ERROR("ゲームオブジェクトコンポーネント(GOCTypeID:", alias_tid.second, ", エイリアス:", alias_tid.first, ")の追加に失敗しました。");
					return false;
				}
			}
			return true;
		}

		bool GameObjectBase::SetDataToComponentsFromPtree_(const boost::property_tree::ptree& pt) {
			for (auto&& com_defs : pt) {
				std::string alias = SeparateAliasAndComponentTypeID(com_defs.first).first;
				auto it = component_holder_.alias_idx_map().find(alias);
				if (it == component_holder_.alias_idx_map().end()) {
					PE_LOG_ERROR("エイリアス\"", alias, "\"が指定されましたが、対応するエイリアスのゲームオブジェクトコンポーネントが存在しません。");
					return false;
				}
				auto cd_pt = com_defs.second;

				//文字列として読み込んでみる
				std::string res_id = cd_pt.get_value<std::string>();
				//文字列が空でなかったらリソースIDとみなす
				if (!res_id.empty()) {
					auto jres = private_::ResourceManager::instance().GetResource<RPtree>(res_id);
					//指定されたリソースがない
					if (jres == nullptr) {
						PE_LOG_ERROR("ゲームオブジェクトコンポーネント(\"エイリアス:", alias, "\")のファイル定義読み込みに失敗しました。指定されたPtreeリソース\"", res_id, "\"を読み込めませんでした。");
						return false;
					}
					cd_pt = *(jres->GetPtree());
				}
				//Ptreeからリフレクションシステムを用いて読み込み
				try {
					component_holder_.component_array()[it->second]->ReflectiveLoadFromPtree(cd_pt);
				} catch (reflection_error& e) {
					PE_LOG_ERROR("ゲームオブジェクトコンポーネント(\"エイリアス:", alias, "\")のファイル定義読み込みに失敗しました。エラーが発生したか、コンポーネントがファイル定義読み込みに対応していない可能性があります。ファイル定義読み込み関数を継承しているか確認してください。(", e.what(), ")");
					return false;
				}
			}
			return  true;
		}

		planeta::GameObjectManagerPublicInterface& GameObjectBase::game_object_manager() {
			return scene_data_->game_object_manager_public_interface;
		}

		std::shared_ptr<GameObjectComponent> GameObjectBase::GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker) const {
			return component_holder_.GetComponentByTypeInfo(ti, type_checker);
		}

		//std::vector<std::shared_ptr<GameObjectComponent>> GameObjectBase::GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker) const {
		//	return std::move(component_holder_.GetAllComponentsByTypeInfo(ti, type_checker));
		//}

		TaskManagerPublicInterface& GameObjectBase::RefTaskManagerInterface_() {
			return scene_data_->task_manager_public_interface;
		}

		void GameObjectBase::SetUpAttachedTask_(TGameObjectOperation& task) {
			task.Attach(GetPointer(), true);
		}

		bool GameObjectBase::ProcessClonation(const std::shared_ptr<GameObjectBase>& dst) {
			//シーンデータのセット
			scene_data_ = dst->scene_data_;
			//コンポーネントのクローン
			dst->component_holder_.CloneToOtherHolder(component_holder_);
			//コンポーネントへシーンとGOデータをセット
			SetSceneAndGODataToCOmponents_();
			return true;
		}

		bool GameObjectBase::AddAndSetUpComponents(const boost::property_tree::ptree& pt) {
			//コンポーネントの追加
			std::vector<std::string> com_type_id_list;
			for (auto&& com_defs : pt) {
				com_type_id_list.push_back(com_defs.first);
			}
			if (!AddComponentsFromTypeIDList_(com_type_id_list)) {
				PE_LOG_ERROR("コンポーネントの作成に失敗しました。");
				return false;
			}
			//コンポーネントのセットアップ
			SetSceneAndGODataToCOmponents_();
			//コンポーネントデータをptreeから読み込み
			if (!SetDataToComponentsFromPtree_(pt)) {
				PE_LOG_ERROR("PtreeからのComponentのデータセットに失敗しました。");
				return false;
			}
			return true;
		}

		bool GameObjectBase::AddAndSetUpComponents(const std::vector<std::string>& com_type_id_list) {
			//コンポーネントの追加
			if (!AddComponentsFromTypeIDList_(com_type_id_list)) {
				PE_LOG_ERROR("コンポーネントの作成に失敗しました。");
				return false;
			}
			//コンポーネントのセットアップ
			SetSceneAndGODataToCOmponents_();
			return true;
		}

		void GameObjectBase::SetSceneAndGODataToCOmponents_() {
			decltype(auto) com_alias_idx_map = component_holder_.alias_idx_map();
			decltype(auto) com_ary = component_holder_.component_array();
			for (auto&& alias_idx : com_alias_idx_map) {
				SetSceneAndGODataToComponent_(*com_ary[alias_idx.second]);
			}
		}
	}
}