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

	GameObjectBase::GameObjectBase() {}

	GameObjectBase::~GameObjectBase() = default;

	std::shared_ptr<IGameObject> GameObjectBase::GetSharedPointer() {
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

	bool GameObjectBase::OnInitialized(const GOComponentGetter&) { return true; }

	bool GameObjectBase::OnActivated() { return true; }

	bool GameObjectBase::OnInactivated() { return true; }

	bool GameObjectBase::OnDisposed() { return true; }

	bool GameObjectBase::ProcessInstantiation() {
		GOComponentAdder gocadder(component_holder_);
		AddComponentsProc(gocadder);
		return true;
	}

	bool GameObjectBase::ProcessLoading(const boost::property_tree::ptree& pt) {
		for (auto&& com_defs : pt) {
			std::string alias = com_defs.first;
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
		return true;
	}

	bool GameObjectBase::ProcessInitialization() {
		GOComponentGetter com_getter(component_holder_);

		decltype(auto) com_alias_idx_map = component_holder_.alias_idx_map();
		decltype(auto) com_ary = component_holder_.component_array();

		for (auto&& alias_idx : com_alias_idx_map) {
			if (!com_ary[alias_idx.second]->GetOtherComponentsProc(com_getter)) {
				PE_LOG_ERROR("GameObjectComponent(\"エイリアス:", alias_idx.first, "\"でほかのオブジェクトを取得する処理に失敗しました。");
				return false;
			}
		}

		if (!OnInitialized(com_getter)) {
			PE_LOG_ERROR("GameObjectの初期化処理に失敗しました。");
			return false;
		}

		for (auto&& alias_idx : com_alias_idx_map) {
			if(!com_ary[alias_idx.second]->Initialize()) {
				PE_LOG_ERROR("GameObjectComponent(\"エイリアス:", alias_idx.first, "\"の初期化処理に失敗しました。");
				return false;
			}
		}
		return true;
	}

	bool GameObjectBase::ProcessActivation() {
		if (!OnActivated()) {
			PE_LOG_ERROR("GameObjectの有効化処理に失敗しました。");
			return false;
		}

		decltype(auto) com_alias_idx_map = component_holder_.alias_idx_map();
		decltype(auto) com_ary = component_holder_.component_array();
		for (auto&& alias_idx : com_alias_idx_map) {
			if (!com_ary[alias_idx.second]->Activate()) {
				PE_LOG_ERROR("GameObjectComponent(\"エイリアス:", alias_idx.first, "\"の有効化処理に失敗しました。");
				return false;
			}
		}
		activated_event_delegate_();
		return true;
	}

	bool GameObjectBase::ProcessInactivation() {
		inactivated_event_delegate_();
		if (!OnInactivated()) {
			PE_LOG_ERROR("GameObjectの無効化処理に失敗しました。");
			return false;
		}

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
		if (!OnDisposed()) {
			PE_LOG_ERROR("GameObjectの破棄処理に失敗しました。");
			return false;
		}
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

	WeakPointer<IGameObject> GameObjectBase::CreateGameObject(const std::string& id, const std::string& resource_id) {
		return scene_data_->game_object_manager_public_interface.CreateGameObject(id, resource_id);
	}

	WeakPointer<IGameObject> GameObjectBase::CreateAndActivateGameObject(const std::string& id, const std::string& resource_id) {
		auto go = scene_data_->game_object_manager_public_interface.CreateGameObject(id, resource_id);
		if (go == nullptr) { return nullptr; }
		go->Activate();
		return go;
	}

	WeakPointer<IGameObject> GameObjectBase::CreateDefaultGameObject(const std::string& id) {
		return scene_data_->game_object_manager_public_interface.CreateDefaultGameObject(id);
	}

	WeakPointer<IGameObject> GameObjectBase::CreateAndActivateDefaultGameObject(const std::string& id) {
		auto go = scene_data_->game_object_manager_public_interface.CreateDefaultGameObject(id);
		if (go == nullptr) { return nullptr; }
		go->Activate();
		return go;
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
		task.Attach(GetSharedPointer(), true);
	}

	bool GameObjectBase::ProcessClonation(const std::shared_ptr<GameObjectBase>& dst) {
		//シーンデータのセット
		scene_data_ = dst->scene_data_;
		//コンポーネントのクローン
		dst->component_holder_.CloneToOtherHolder(component_holder_);
		//コンポーネントへシーンとGOデータをセット
		SetSceneAndGODataToCOmponents();
		return true;
	}

	void GameObjectBase::SetSceneAndGODataToCOmponents() {
		decltype(auto) com_alias_idx_map = component_holder_.alias_idx_map();
		decltype(auto) com_ary = component_holder_.component_array();
		for (auto&& alias_idx : com_alias_idx_map) {
			SetSceneAndGODataToComponent_(*com_ary[alias_idx.second]);
		}
	}

	void GameObjectBase::AddComponentsProc(GOComponentAdder& com_adder) {}

}