#include "GameObject.h"

#include"GameObjectManager.h"
#include "Component.h"
#include "GameObjectSetUpProxy.h"
#include "GameObjectSetUpper.h"
#include "PlanetComponent.h"

using namespace std;

namespace PlanetaEngine{
	namespace Game{

		GameObject::GameObject() :_is_active(false), _id(-1), _game_object_set_up_proxy(new GameObjectSetUpProxy(*this)),_component_id_counter(0),_game_object_manager(nullptr),_process_manager(nullptr){};

		GameObject::~GameObject(){
			_Finalize();
			delete _game_object_set_up_proxy;
		}

		std::shared_ptr<GameObject> GameObject::Create(GameObjectSetUpper& gosu){
			auto h_ptr = MakeShared<create_helper>();
			auto ptr = std::shared_ptr<GameObject>(std::move(h_ptr), &h_ptr->x);
			ptr->_me = ptr;
			if (ptr->_SetUp(gosu) == false){ return nullptr; } //Ý’è
			if (ptr->_Initialize() == false){ return nullptr; } //‰Šú‰»
			return ptr;
		}

		void GameObject::Activate()
		{
			_id = game_object_manager().Resist(me());
			_Activated();
			_is_active = true;
		}

		void GameObject::Activate(const std::string& name)
		{
			_id = game_object_manager().Resist(me(), name);
			_Activated();
			_is_active = true;
		}

		void GameObject::InActivate()
		{
			game_object_manager().Unresist(_id);
			_id = -1;
			_InActivated();
			_is_active = false;
		}

		bool GameObject::_initialize_component()
		{
			for (auto& c : _components){
				if (c.second->Initialize() == false){ return false; }
			}
			return true;
		}

		void GameObject::_update_component()
		{
			for (auto& c : _components){
				c.second->Update();
			}
		}

		bool GameObject::_finalize_component()
		{
			for (auto& c : _components){
				c.second->Finalize();
			}
			return true;
		}

		bool GameObject::_activate_component()
		{
			for (auto& c : _components){
				if (c.second->Activated() == false){ return false; }
			}
			return true;
		}

		bool GameObject::_inactivate_component()
		{
			for (auto& c : _components){
				if (c.second->InActivated() == false){ return false; }
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

	}
}