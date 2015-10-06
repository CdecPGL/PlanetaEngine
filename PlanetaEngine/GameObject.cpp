#include "GameObject.h"

#include"GameObjectManager.h"
#include "Component.h"
#include "GameObjectSetUpProxy.h"
#include "GameObjectSetUpper.h"
#include "PlanetComponent.h"
#include "GameObjectManager.h"
#include "GameProcessManager.h"
#include "TransformComponent.h"
#include "SystemLog.h"
#include "ISceneAccessForGameObject.h"
#include "DumyGroundComponent.h"

using namespace std;

namespace planeta_engine{
	namespace game{

		GameObject::GameObject() :_is_active(false), _id(-1), _game_object_set_up_proxy(std::make_unique<GameObjectSetUpProxy>(*this)),_component_id_counter(0)
			,_transform(components::TransformComponent::MakeShared<components::TransformComponent>()){
			_transform->parent(GetRootTransformComponent()); //トランスフォームコンポーネントの親にRootを設定
		};

		GameObject::~GameObject(){
		}

		std::shared_ptr<GameObject> GameObject::Create(GameObjectSetUpper& gosu){
			auto h_ptr = MakeShared<create_helper>();
			auto ptr = std::shared_ptr<GameObject>(std::move(h_ptr), &h_ptr->x);
			ptr->_me = ptr;
			ptr->_transform->SetGameObject(ptr, ptr->_component_id_counter++);
			if (ptr->_SetUp(gosu) == false){  //設定
				debug::SystemLog::instance().LogError("ゲームオブジェクトの作成に失敗しました。セットアップに失敗しました。", "GameObject::Create");
				return nullptr;
			}
			return ptr;
		}

		bool GameObject::Activate()
		{
			if (_scene) {
				if (!static_cast<GameObjectManager&>(_scene->game_object_manager()).Activate(_id)) { return false; }
				_is_active = true;
				return true;
			}
			else {
				//ゲームオブジェクトマネージャがセットされていない
				debug::SystemLog::instance().LogError("ゲームオブジェクトの有効化に失敗しました。Sceneがセットされていません。", "GameObject::Activate");
				return false;
			}
		}

		bool GameObject::InActivate()
		{
			if (_scene) {
				if (!static_cast<GameObjectManager&>(_scene->game_object_manager()).InActivate(_id)) { return false; }
				_is_active = false;
				return true;
			}
			else {
				//ゲームオブジェクトマネージャがセットされていない
				debug::SystemLog::instance().LogError("ゲームオブジェクトの無効化に失敗しました。Sceneがセットされていません。","GameObject::InActivate");
				return false;
			}
		}

		bool GameObject::_initialize_component()
		{
			for (auto& c : _component_list){
				c.second->SetSceneAccessor(_scene);
				if (c.second->Initialize_() == false){ 
					debug::SystemLog::instance().LogError(std::string("コンポーネントの初期化に失敗しました。(") + c.second->GetType().name()+")", "GameObject::_initialize_component");
					return false; 
				}
			}
			return true;
		}

		void GameObject::_update_component()
		{
			//通常コンポーネントのアップデート
			for (auto& c : _component_update_list){
				c->Update_();
			}
		}

		bool GameObject::_finalize_component()
		{
			for (auto& c : _component_list){
				c.second->Finalize_();
			}
			return true;
		}

		bool GameObject::_activate_component()
		{
			for (auto& c : _component_list){
				if (c.second->Activated_() == false){ 
					debug::SystemLog::instance().LogError(std::string("コンポーネントの有効化に失敗しました。(") + c.second->GetType().name()+")", "GameObject::_activate_component");
					return false;
				}
			}
			return true;
		}

		bool GameObject::_inactivate_component()
		{
			for (auto& c : _component_list){
				if (c.second->InActivated_() == false){ 
					debug::SystemLog::instance().LogError(std::string("コンポーネントの無効化に失敗しました。(") + c.second->GetType().name()+")", "GameObject::_inactivate_component");
					return false; 
				}
			}
			return true;
		}

//		std::shared_ptr<GameObject> GameObject::Clone() const
//		{
//			std::shared_ptr<GameObject> clone = Create();
//			for (const auto& c : _components){
////				
//			}
//			if (clone->_Initialize() == false){ return nullptr; }
//			else{ return clone; }
//		}

		bool GameObject::_SetUp(GameObjectSetUpper& gosu)
		{
			return gosu(*_game_object_set_up_proxy);
		}

		void GameObject::Dispose()
		{
			if (_scene) {
				static_cast<GameObjectManager&>(_scene->game_object_manager()).Remove(_id);
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

	}
}