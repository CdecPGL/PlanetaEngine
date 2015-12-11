#pragma once

#include"Object.h"
#include "MakeSharedGameObject.h"
#include "IGameObjectAccessor.h"
#include "GameObjectResisterConnection.h"
#include<vector>
#include <unordered_map>

namespace planeta_engine{
	namespace core{
		class SceneAccessorForGameObject;
	}
	namespace components {
		class GroundComponent;
		class TransformComponent;
	}
	namespace game{
		class GameObjectComponent;
		class GameObjectSetUpProxy; //初期化用関数使用仲介クラス
		class GameObjectSetUpper; //初期化クラス
		//GameObjectクラス(継承禁止ぃ)
		class GameObject final : public core::Object,public IGameObjectAccessor{
			friend GameObjectSetUpProxy;
		public:
			//////////ユーザアクセス可能関数//////////
			//有効化(ゲームシーンに登録する)
			bool Activate()override;
			bool InActivate()override; //無効化(ゲームシーンから登録解除する)
			/*破棄*/
			void Dispose()override;
			//アクセサ
			bool is_active()const override { return is_active_; }
			//指定IDのコンポーネント取得
			utility::WeakPointer<GameObjectComponent> GetComponent(int id) override{
				auto it = component_list_.find(id);
				if (it == component_list_.end()) { return nullptr; }
				else { return it->second; }
			}
			/*特殊コンポーネント取得*/
			/*所属地形取得*/
			utility::WeakPointer<components::GroundComponent> GetBelongingGroundComponent()override { return belonging_ground_ ? belonging_ground_ : GetDumyGroundComponent_(); }
			const components::GroundComponent& belonging_ground()const override{ return belonging_ground_ ? *belonging_ground_ : *GetDumyGroundComponent_(); }
			components::GroundComponent& belonging_ground()override { return belonging_ground_ ? *belonging_ground_ : *GetDumyGroundComponent_(); }
			/*ローカルトランスフォーム取得*/
			utility::WeakPointer<components::TransformComponent> GetTransformComponent()override { return transform_; }
			const components::TransformComponent& transform()const override{ return *transform_; }
			components::TransformComponent& transform()override { return *transform_; }
			/*所属地形をセット*/
			void SetBelongingGround(const utility::WeakPointer<components::GroundComponent>& belonging_ground)override { belonging_ground_ = belonging_ground; }

			////////////システム関数//////////
			//作成
			static std::shared_ptr<GameObject> Create(GameObjectSetUpper& gosu);

			/*シーンのポインタをセット*/
			void SetSceneAccessor(const utility::WeakPointer<core::SceneAccessorForGameObject>& scene) { scene_accessor_ = scene; }
			//自身のクローン作成(親子関係はリセットされる)
//			std::shared_ptr<GameObject> Clone()const;

			//更新(マイフレーム呼ばれる)[呼び出し元:GameObjectUpdateProcess::Update]
			void Update(){
				_update_component();	
			};
			class GameObjectManagementProxy; //管理用関数使用仲介クラス
			//システムに呼ばれる関数群
			//初期化(作成時に呼ばれる)[呼び出し元:GameObjectManager::Resist]
			bool _Initialize(std::unique_ptr<GameObjectResisterConnection>&& rc) { resister_connection_ = std::move(rc); return  _initialize_component(); };
			//シーン登録時に呼ばれる[呼び出し元:GameObjectManager::Activate]
			bool _Activated() { return _activate_component(); }
			//シーン登録解除時に呼ばれる[呼び出し元:GameObjectManager::InActivate]
			bool _InActivated() { return _inactivate_component(); }
			//終了処理(破棄時に呼ばれる)[呼び出し元:GameObjectManager::Remove]
			bool _Finalize()throw() { return _finalize_component(); };

			/*Rootとなるトランスフォームコンポーネントを取得*/
			static std::shared_ptr<components::TransformComponent> GetRootTransformComponent(bool recreate_flag = false);
			/*弱参照を取得*/
			utility::WeakPointer<IGameObjectAccessor> GetWeakPointer()override { return me(); };
		protected:
			std::shared_ptr<GameObject> me(){ return me_.lock(); } //自分のスマポ取得
		private:
			//直接生成、コピーの禁止
			//既定のコンストラクタ
			GameObject();
			~GameObject();
			
			GameObject(const GameObject&) = delete;
			GameObject(GameObject&&) = delete;
			const GameObject& operator=(const GameObject&) = delete;
			const GameObject& operator=(GameObject&&) = delete;

//			static void* operator new(size_t s){ return Object::operator new(s); }
//			static void operator delete(void* p){ return Object::operator delete(p); }
//			static void* operator new[](size_t s){ return Object::operator new[](s); }
//			static void operator delete[](void* p){ return Object::operator delete[](p); }

			utility::WeakPointer<core::SceneAccessorForGameObject> scene_accessor_;
			std::unique_ptr<GameObjectResisterConnection> resister_connection_;

			std::weak_ptr<GameObject> me_; //自分のスマートポインタ
			std::unordered_map<int,std::shared_ptr<GameObjectComponent>> component_list_; //コンポーネントリスト
			const std::unordered_map<int, std::shared_ptr<GameObjectComponent>>& _GetComponentList()const override { return component_list_; }
			std::vector<std::shared_ptr<GameObjectComponent>> component_update_list_; //コンポーネント更新リスト

			//////////特殊コンポーネント//////////
			std::shared_ptr<components::TransformComponent> transform_; //ローカル形状情報
			utility::WeakPointer<components::GroundComponent> belonging_ground_; //所属地形(コンストラクタでダミーがセットされる)

			bool is_active_;

			bool _initialize_component();
			void _update_component();
			bool _finalize_component();
			bool _activate_component();
			bool _inactivate_component();
			template<class C>
			utility::WeakPointer<C> _add_component(){ 
				static_assert(std::is_base_of<GameObjectComponent, C>::value == true, "C is not derived Component.");
				auto ptr = C::MakeShared<C>();
				ptr->SetGameObject(me(), component_id_counter_);
				component_list_.insert(std::make_pair(component_id_counter_++, ptr));
				//更新処理を行う場合は更新リストに登録
				std::shared_ptr<GameObjectComponent> com_ptr = ptr;
				if (!com_ptr->NoUpdate_()) { component_update_list_.push_back(ptr); }
				return ptr;
			}
			int component_id_counter_;
			std::unique_ptr<GameObjectSetUpProxy> game_object_set_up_proxy_;
			struct create_helper;
			//セットアップ(Initializeまえによばれる)[呼び出し元:GameObject::Create]
			bool _SetUp(GameObjectSetUpper& gosu);

			/*ダミーの地形コンポーネントを取得*/
			static std::shared_ptr<components::GroundComponent> GetDumyGroundComponent_();
		};
		struct GameObject::create_helper {
			GameObject x;
			// クラス内クラスだとテンプレートにできるのでテンプレートで書いてみる
			template<class... Args>
			explicit create_helper(Args&&... args)
				: x(std::forward<Args>(args)...) {
			}
			~create_helper(){}
		};
	}
}
