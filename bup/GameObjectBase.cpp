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
#include "RJson.h"
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

	bool GameObjectBase::OnInitialized() { return true; }

	bool GameObjectBase::OnActivated() { return true; }

	bool GameObjectBase::OnInactivated() { return true; }

	bool GameObjectBase::OnDisposed() { return true; }

	std::shared_ptr<GameObjectBase> GameObjectBase::Clone() {
		auto ptr = Reflection::CreateObjectByStdTypeInfo<GameObjectBase>(typeid(*this));
		assert(ptr != nullptr);
		component_holder_.CloneToOtherHolder(ptr->component_holder_);
		return ptr;
	}

	bool GameObjectBase::ProcessInstantiation() {
		GOComponentAdder gocadder(component_holder_);
		AddComponentsProc(gocadder);
		return true;
	}

	bool GameObjectBase::ProcessLoading(const JSONObject& json_object) {
		for (auto&& com_defs : json_object) {
			std::string alias = com_defs.first;
			auto it = component_holder_.alias_map().find(alias);
			if (it == component_holder_.alias_map().end()) {
				PE_LOG_ERROR("エイリアス\"", alias, "\"が指定されましたが、対応するエイリアスのゲームオブジェクトコンポーネントが存在しません。");
				return false;
			}
			auto jval = com_defs.second;
			//JsonObjectとして読み込んでみる
			auto jobj = jval->Get<JSONObject>();
			//だめならstd::stringとして読み込んでみる
			if (jobj == nullptr) {
				auto jstr = jval->Get<std::string>();
				if (jstr) {
					std::string res_id = *jstr;
					auto jres = core::ResourceManager::instance().GetResource<RJson>(res_id);
					//指定されたリソースがない
					if (jres == nullptr) {
						PE_LOG_ERROR("ゲームオブジェクトコンポーネント(\"エイリアス:", alias, "\")のファイル定義読み込みに失敗しました。指定されたJsonリソース\"", res_id, "\"を読み込めませんでした。");
						return false;
					}
					jobj = jres->GetRoot().Get<JSONObject>();
					//指定JsonファイルのルートがObjectでない
					if (jobj == nullptr) {
						PE_LOG_ERROR("ゲームオブジェクトコンポーネント(\"エイリアス:", alias, "\")の定義ファイル\"", res_id, "\"のルートはJsonObject型でなければいけません。");
						return false;
					}
				} else { //JObjでもstd::stringでもだめだったらエラー
					PE_LOG_ERROR("ゲームオブジェクトコンポーネント定義(\"エイリアス:", alias, "\")の型が不正です。JSONObject型で直接定義するか、文字列型で定義ファイルを指定してください。");
					return false;
				}
			}
			//JsonObjの取得に成功したので読み込み
			if (!it->second->Load(*jobj)) {
				PE_LOG_ERROR("ゲームオブジェクトコンポーネント(\"エイリアス:", alias, "\")のファイル定義読み込みに失敗しました。エラーが発生したか、コンポーネントがファイル定義読み込みに対応していない可能性があります。ファイル定義読み込み関数を継承しているか確認してください。");
				return false;
			}
		}
		return true;
	}

	bool GameObjectBase::ProcessInitialization() {
		if (!OnInitialized()) {
			PE_LOG_ERROR("GameObjectの初期化処理に失敗しました。");
			return false;
		}
		GOComponentGetter com_getter(component_holder_);
		for (auto&& com : component_holder_.alias_map()) {
			if(!com.second->Initialize(com_getter)) {
				PE_LOG_ERROR("GameObjectComponent(\"エイリアス:", com.first, "\"の初期化処理に失敗しました。");
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
		for (auto&& com : component_holder_.alias_map()) {
			if (!com.second->Activate()) {
				PE_LOG_ERROR("GameObjectComponent(\"エイリアス:", com.first, "\"の有効化処理に失敗しました。");
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
		for (auto&& com : component_holder_.alias_map()) {
			if (!com.second->InActivate()) {
				PE_LOG_ERROR("GameObjectComponent(\"エイリアス:", com.first, "\"の無効化処理に失敗しました。");
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
		for (auto&& com : component_holder_.component_list()) {
			com->Finalize();
		}
		return true;
	}

	void GameObjectBase::SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn) {
		manager_connection_ = std::move(mgr_cnctn);
	}

	void GameObjectBase::SetSceneData(const util::WeakPointer<core::SceneData>& scene_accessor) {
		scene_data_ = scene_accessor;
	}

	util::WeakPointer<IGameObject> GameObjectBase::CreateGameObject(const std::string& id, const std::string& resource_id) {
		return scene_data_->game_object_manager_public_interface.CreateGameObject(id, resource_id);
	}

	util::WeakPointer<IGameObject> GameObjectBase::CreateAndActivateGameObject(const std::string& id, const std::string& resource_id) {
		auto go = scene_data_->game_object_manager_public_interface.CreateGameObject(id, resource_id);
		if (go == nullptr) { return nullptr; }
		go->Activate();
		return go;
	}

	util::WeakPointer<IGameObject> GameObjectBase::CreateDefaultGameObject(const std::string& id) {
		return scene_data_->game_object_manager_public_interface.CreateDefaultGameObject(id);
	}

	util::WeakPointer<IGameObject> GameObjectBase::CreateAndActivateDefaultGameObject(const std::string& id) {
		auto go = scene_data_->game_object_manager_public_interface.CreateDefaultGameObject(id);
		if (go == nullptr) { return nullptr; }
		go->Activate();
		return go;
	}

	util::DelegateConnection GameObjectBase::AddActivatedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder) {
		return hander_adder(activated_event_delegate_);
	}

	util::DelegateConnection GameObjectBase::AddInactivatedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder) {
		return hander_adder(inactivated_event_delegate_);
	}

	util::DelegateConnection GameObjectBase::AddDisposedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder) {
		return hander_adder(disposed_event_delegate_);
	}

	void GameObjectBase::SetUpGameComponent(GameObjectComponent& com) {
		core::GameObjectComponentSetUpData rd{ this, scene_data_ };
		com.SystemSetUp(rd);
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
}