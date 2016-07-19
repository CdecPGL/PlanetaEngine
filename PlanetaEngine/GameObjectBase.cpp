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
		return nullptr;
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
				PE_LOG_ERROR("�G�C���A�X\"", alias, "\"���w�肳��܂������A�Ή�����G�C���A�X�̃Q�[���I�u�W�F�N�g�R���|�[�l���g�����݂��܂���B");
				return false;
			}
			auto jval = com_defs.second;
			//JsonObject�Ƃ��ēǂݍ���ł݂�
			auto jobj = jval->Get<JSONObject>();
			//���߂Ȃ�std::string�Ƃ��ēǂݍ���ł݂�
			if (jobj == nullptr) {
				auto jstr = jval->Get<std::string>();
				if (jstr) {
					std::string res_id = *jstr;
					auto jres = core::ResourceManager::instance().GetResource<RJson>(res_id);
					//�w�肳�ꂽ���\�[�X���Ȃ�
					if (jres == nullptr) {
						PE_LOG_ERROR("�Q�[���I�u�W�F�N�g�R���|�[�l���g(\"�G�C���A�X:", alias, "\")�̃t�@�C����`�ǂݍ��݂Ɏ��s���܂����B�w�肳�ꂽJson���\�[�X\"", res_id, "\"��ǂݍ��߂܂���ł����B");
						return false;
					}
					jobj = jres->GetRoot().Get<JSONObject>();
					//�w��Json�t�@�C���̃��[�g��Object�łȂ�
					if (jobj == nullptr) {
						PE_LOG_ERROR("�Q�[���I�u�W�F�N�g�R���|�[�l���g(\"�G�C���A�X:", alias, "\")�̒�`�t�@�C��\"", res_id, "\"�̃��[�g��JsonObject�^�łȂ���΂����܂���B");
						return false;
					}
				} else { //JObj�ł�std::string�ł����߂�������G���[
					PE_LOG_ERROR("�Q�[���I�u�W�F�N�g�R���|�[�l���g��`(\"�G�C���A�X:", alias, "\")�̌^���s���ł��BJSONObject�^�Œ��ڒ�`���邩�A������^�Œ�`�t�@�C�����w�肵�Ă��������B");
					return false;
				}
			}
			//JsonObj�̎擾�ɐ��������̂œǂݍ���
			if (!it->second->Load(*jobj)) {
				PE_LOG_ERROR("�Q�[���I�u�W�F�N�g�R���|�[�l���g(\"�G�C���A�X:", alias, "\")�̃t�@�C����`�ǂݍ��݂Ɏ��s���܂����B�G���[�������������A�R���|�[�l���g���t�@�C����`�ǂݍ��݂ɑΉ����Ă��Ȃ��\��������܂��B�t�@�C����`�ǂݍ��݊֐����p�����Ă��邩�m�F���Ă��������B");
				return false;
			}
		}
		return true;
	}

	bool GameObjectBase::ProcessInitialization() {
		if (!OnInitialized()) {
			PE_LOG_ERROR("GameObject�̏����������Ɏ��s���܂����B");
			return false;
		}
		GOComponentGetter com_getter(component_holder_);
		for (auto&& com : component_holder_.alias_map()) {
			if(!com.second->Initialize(com_getter)) {
				PE_LOG_ERROR("GameObjectComponent(\"�G�C���A�X:", com.first, "\"�̏����������Ɏ��s���܂����B");
				return false;
			}
		}
		return true;
	}

	bool GameObjectBase::ProcessActivation() {
		if (!OnActivated()) {
			PE_LOG_ERROR("GameObject�̗L���������Ɏ��s���܂����B");
			return false;
		}
		for (auto&& com : component_holder_.alias_map()) {
			if (!com.second->Activate()) {
				PE_LOG_ERROR("GameObjectComponent(\"�G�C���A�X:", com.first, "\"�̗L���������Ɏ��s���܂����B");
				return false;
			}
		}
		activated_event_delegate_();
		return true;
	}

	bool GameObjectBase::ProcessInactivation() {
		inactivated_event_delegate_();
		if (!OnInactivated()) {
			PE_LOG_ERROR("GameObject�̖����������Ɏ��s���܂����B");
			return false;
		}
		for (auto&& com : component_holder_.alias_map()) {
			if (!com.second->InActivate()) {
				PE_LOG_ERROR("GameObjectComponent(\"�G�C���A�X:", com.first, "\"�̖����������Ɏ��s���܂����B");
				return false;
			}
		}
		return true;
	}

	bool GameObjectBase::ProcessDisposal() {
		disposed_event_delegate_();
		if (!OnDisposed()) {
			PE_LOG_ERROR("GameObject�̔j�������Ɏ��s���܂����B");
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