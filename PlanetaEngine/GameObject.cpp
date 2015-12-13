#include "GameObject.h"

#include "GameObjectComponent.h"
#include "GameObjectSetUpProxy.h"
#include "GameObjectSetUpper.h"
#include "PlanetComponent.h"
#include "TransformComponent.h"
#include "SystemLog.h"
#include "SceneAccessorForGameObject.h"
#include "DumyGroundComponent.h"
#include "GameObjectComponentRigistrationData.h"

using namespace std;

namespace planeta_engine{
	namespace game{

		GameObject::GameObject() :is_active_(false), game_object_set_up_proxy_(std::make_unique<GameObjectSetUpProxy>(*this)),component_id_counter_(0)
			,transform_(components::TransformComponent::MakeShared<components::TransformComponent>()){
			transform_->parent(GetRootTransformComponent()); //トランスフォームコンポーネントの親にRootを設定
		};

		GameObject::~GameObject() = default;

		std::shared_ptr<GameObject> GameObject::Create(GameObjectSetUpper& gosu){
			auto h_ptr = MakeShared<create_helper>();
			auto ptr = std::shared_ptr<GameObject>(std::move(h_ptr), &h_ptr->x);
			ptr->me_ = ptr;
			ptr->SystemSetUpComponent_(*ptr->transform_);
			if (ptr->SetUp_(gosu) == false){  //設定
				debug::SystemLog::instance().LogError("ゲームオブジェクトの作成に失敗しました。セットアップに失敗しました。",__FUNCTION__);
				return nullptr;
			}
			return ptr;
		}

		bool GameObject::Activate()
		{
			if (scene_accessor_) {
				if (!resister_connection_->Activate()) { return false; }
				is_active_ = true;
				return true;
			}
			else {
				//ゲームオブジェクトマネージャがセットされていない
				debug::SystemLog::instance().LogError("ゲームオブジェクトの有効化に失敗しました。Sceneがセットされていません。", __FUNCTION__);
				return false;
			}
		}

		bool GameObject::InActivate()
		{
			if (scene_accessor_) {
				if (!resister_connection_->InActivate()) { return false; }
				is_active_ = false;
				return true;
			}
			else {
				//ゲームオブジェクトマネージャがセットされていない
				debug::SystemLog::instance().LogError("ゲームオブジェクトの無効化に失敗しました。Sceneがセットされていません。",__FUNCTION__);
				return false;
			}
		}

		bool GameObject::InitializeComponent_()
		{
			for (auto& c : component_list_){
				if (c.second->Initialize() == false){ 
					debug::SystemLog::instance().LogError(std::string("コンポーネントの初期化に失敗しました。(") + c.second->GetType().name()+")",__FUNCTION__);
					return false; 
				}
			}
			return true;
		}

		void GameObject::UpdateComponent_()
		{
			//通常コンポーネントのアップデート
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
					debug::SystemLog::instance().LogError(std::string("コンポーネントの有効化に失敗しました。(") + c.second->GetType().name()+")", __FUNCTION__);
					return false;
				}
			}
			return true;
		}

		bool GameObject::InactivateComponent_()
		{
			for (auto& c : component_list_){
				if (c.second->InActivate() == false){ 
					debug::SystemLog::instance().LogError(std::string("コンポーネントの無効化に失敗しました。(") + c.second->GetType().name()+")", __FUNCTION__);
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

		bool GameObject::SetUp_(GameObjectSetUpper& gosu)
		{
			return gosu(*game_object_set_up_proxy_);
		}

		void GameObject::Dispose()
		{
			if (scene_accessor_) {
				resister_connection_->Dispose();
			}
			else {
				//ゲームオブジェクトマネージャがセットされていない
				debug::SystemLog::instance().LogError("ゲームオブジェクトの破棄に失敗しました。Sceneがセットされていません。", __FUNCTION__);
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
			bool ret = com.SystemSetUp(registration_data, resister_connection_->RefSceneData());
			assert(ret == true);
		}

	}
}