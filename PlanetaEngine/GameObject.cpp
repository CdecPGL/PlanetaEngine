#include "GameObject.h"

#include "GameObjectComponent.h"
#include "GameObjectAccessorForSetUp.h"
#include "PlanetComponent.h"
#include "TransformComponent.h"
#include "SystemLog.h"
#include "SceneAccessorForGameObject.h"
#include "DumyGroundComponent.h"
#include "GameObjectComponentRigistrationData.h"

using namespace std;

namespace planeta_engine{
	namespace game{

		GameObject::GameObject() :is_active_(false),component_id_counter_(0)
			,transform_(components::TransformComponent::MakeShared<components::TransformComponent>()){
		};

		GameObject::~GameObject() = default;

		std::shared_ptr<GameObject> GameObject::Create(){
			auto h_ptr = MakeShared<create_helper>();
			auto ptr = std::shared_ptr<GameObject>(std::move(h_ptr), &h_ptr->x);
			ptr->me_ = ptr;
			return ptr;
		}

		bool GameObject::Activate()
		{
			if (scene_accessor_) {
				if (!manager_connection_->Activate()) { return false; }
				is_active_ = true;
				return true;
			}
			else {
				//�Q�[���I�u�W�F�N�g�}�l�[�W�����Z�b�g����Ă��Ȃ�
				debug::SystemLog::instance().LogError("�Q�[���I�u�W�F�N�g�̗L�����Ɏ��s���܂����BScene���Z�b�g����Ă��܂���B", __FUNCTION__);
				return false;
			}
		}

		bool GameObject::InActivate()
		{
			if (scene_accessor_) {
				if (!manager_connection_->InActivate()) { return false; }
				is_active_ = false;
				return true;
			}
			else {
				//�Q�[���I�u�W�F�N�g�}�l�[�W�����Z�b�g����Ă��Ȃ�
				debug::SystemLog::instance().LogError("�Q�[���I�u�W�F�N�g�̖������Ɏ��s���܂����BScene���Z�b�g����Ă��܂���B",__FUNCTION__);
				return false;
			}
		}

		bool GameObject::InitializeComponent_()
		{
			for (auto& c : component_list_){
				if (c.second->Initialize() == false){ 
					debug::SystemLog::instance().LogError(std::string("�R���|�[�l���g�̏������Ɏ��s���܂����B(") + c.second->GetType().name()+")",__FUNCTION__);
					return false; 
				}
			}
			return true;
		}

		void GameObject::UpdateComponent_()
		{
			//�ʏ�R���|�[�l���g�̃A�b�v�f�[�g
			for (auto& c : component_update_list_){
				c->Update();
			}
		}

		bool GameObject::FinalizeComponent_()
		{
			for (auto& c : component_list_){
				c.second->Finalize();
			}
			return true;
		}

		bool GameObject::ActivateComponent_()
		{
			for (auto& c : component_list_){
				if (c.second->Activate() == false){ 
					debug::SystemLog::instance().LogError(std::string("�R���|�[�l���g�̗L�����Ɏ��s���܂����B(") + c.second->GetType().name()+")", __FUNCTION__);
					return false;
				}
			}
			return true;
		}

		bool GameObject::InactivateComponent_()
		{
			for (auto& c : component_list_){
				if (c.second->InActivate() == false){ 
					debug::SystemLog::instance().LogError(std::string("�R���|�[�l���g�̖������Ɏ��s���܂����B(") + c.second->GetType().name()+")", __FUNCTION__);
					return false; 
				}
			}
			return true;
		}

		void GameObject::Dispose()
		{
			if (scene_accessor_) {
				manager_connection_->Dispose();
			}
			else {
				//�Q�[���I�u�W�F�N�g�}�l�[�W�����Z�b�g����Ă��Ȃ�
				debug::SystemLog::instance().LogError("�Q�[���I�u�W�F�N�g�̔j���Ɏ��s���܂����BScene���Z�b�g����Ă��܂���B", __FUNCTION__);
			}
		}

		std::shared_ptr<components::GroundComponent> GameObject::GetDumyGroundComponent_()
		{
			static std::shared_ptr<components::DumyGroundComponent> dgc = std::make_shared<components::DumyGroundComponent>();
			return dgc;
		}

		std::shared_ptr<components::TransformComponent> GameObject::GetRootTransformComponent(bool recreate_flag)
		{
			static std::shared_ptr<components::TransformComponent> rtc;
			if (recreate_flag) {
				rtc = std::make_shared<components::TransformComponent>();
			}
			return rtc;
		}

		void GameObject::SystemSetUpComponent_(GameObjectComponent& com) {
			core::GameObjectComponentRegistrationData registration_data;
			registration_data.holder_game_object = me();
			registration_data.scene_accessor = scene_accessor_;
			registration_data.id = component_id_counter_++;
			bool ret = com.SystemSetUp(registration_data, manager_connection_->RefSceneData());
			assert(ret == true);
		}

		bool GameObject::SetUpSystemComponent() {
			SystemSetUpComponent_(*transform_);
			return true;
		}

		std::shared_ptr<GameObjectComponent> GameObject::GetComponentByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker) const {
			auto it = component_type_map_.find(ti);
			if (it == component_type_map_.end()) {
				for (const auto& c : component_list_) {
					//���݂�����^�C�v���X�g�ɒǉ����ĕs���S�}�[�N���Ċ֐��𔲂���
					if (type_checker(c.second.get())) {
						component_type_map_.emplace(ti, std::make_pair(false, std::vector<std::shared_ptr<GameObjectComponent>>{ c.second }));
						return c.second;
					}
				}
				//���݂��Ȃ�������^�C�v���X�g����ɂ��Ċ��S�}�[�N���Ċ֐��𔲂���
				component_type_map_.emplace(ti, std::make_pair(true, std::vector<std::shared_ptr<GameObjectComponent>>()));
				return nullptr;
			} else {
				const auto& cl = it->second.second;
				return cl.size() > -0 ? cl[0] : nullptr;
			}
		}

		const std::vector<std::shared_ptr<GameObjectComponent>>& GameObject::GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker) const {
			auto it = component_type_map_.find(ti);
			auto all_search = [this,&type_checker,&ti](std::vector<std::shared_ptr<GameObjectComponent>>& t_list) {
				for (const auto& c : component_list_) {
					//���݂�����^�C�v���X�g�ɒǉ�
					if (type_checker(c.second.get())) {
						t_list.push_back(c.second);
					}
				}
				return t_list;
			};

			if (it == component_type_map_.end()) { //�^�C�v�̒T�����s���Ă��Ȃ�������
				auto& t_list = component_type_map_.emplace(ti, std::make_pair(true, std::vector<std::shared_ptr<GameObjectComponent>>())).first->second.second;
				all_search(t_list); //�S�T������
				return t_list;
			} else {
				if (it->second.first) { //�T�����I�����Ă�����
					return it->second.second;
				} else { //�����T���������Ă��Ȃ�������
					all_search(it->second.second); //�S�T������
					return it->second.second;
				}
			}
		}

		void GameObject::AddComponentToTypeInfoMap(const std::type_info& ti, const std::shared_ptr<GameObjectComponent>& com) {
			auto it = component_type_map_.find(ti);
			if (it == component_type_map_.end()) {
				component_type_map_.emplace(ti, std::make_pair(false, std::vector<std::shared_ptr<GameObjectComponent>>({ com })));
			} else {
				assert(component_type_map_[ti].first == false);
				component_type_map_[ti].second.push_back(com);
			}
		}

	}
}