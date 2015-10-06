#ifndef DEF_OBJECT_H //二重include防止

#define DEF_OBJECT_H

#include"Object.h"
//#include "ObjectHolderTemplate_WithoutID.h"
//#include "GameObjectAllocator.h"
#include "MakeSharedGameObject.h"
#include "IGameObjectAccessor.h"
#include<vector>
#include <unordered_map>

namespace planeta_engine{
	namespace core{
		class ISceneAccessForGameObject;
	}
	namespace components {
		class GroundComponent;
		class TransformComponent;
	}
	namespace game{
		class GameObjectManager;
		class GameProcessManager;
		class Component;
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
			bool is_active()const override { return _is_active; }
			int ID()const override { return _id; }
			//コンポーネント関連
			//特定型のコンポーネント取得
			/*template<class C>
			std::shared_ptr<C> GetComponent()const override{
				for (const auto& com : _components) {
					auto ptr = dynamic_pointer_cast<C>(com.second);
					if (ptr != nullptr) { return ptr; }
				}
				return nullptr;
			}*/
			//指定IDのコンポーネント取得
			utility::WeakPointer<Component> GetComponent(int id) override{
				auto it = _component_list.find(id);
				if (it == _component_list.end()) { return nullptr; }
				else { return it->second; }
			}
			/*特殊コンポーネント取得*/
			/*所属地形取得*/
			utility::WeakPointer<components::GroundComponent> GetBelongingGroundComponent()override { return _belonging_ground ? _belonging_ground : GetDumyGroundComponent_(); }
			const components::GroundComponent& belonging_ground()const override{ return _belonging_ground ? *_belonging_ground : *GetDumyGroundComponent_(); }
			components::GroundComponent& belonging_ground()override { return _belonging_ground ? *_belonging_ground : *GetDumyGroundComponent_(); }
			/*ローカルトランスフォーム取得*/
			utility::WeakPointer<components::TransformComponent> GetTransformComponent()override { return _transform; }
			const components::TransformComponent& transform()const override{ return *_transform; }
			components::TransformComponent& transform()override { return *_transform; }
			/*所属地形をセット*/
			void SetBelongingGround(const utility::WeakPointer<components::GroundComponent>& belonging_ground)override { _belonging_ground = belonging_ground; }

			////////////システム関数//////////
			//作成
			static std::shared_ptr<GameObject> Create(GameObjectSetUpper& gosu);

			/*シーンのポインタをセット*/
			void SetSceneAccessor(const utility::WeakPointer<core::ISceneAccessForGameObject>& scene) { _scene = scene; }
			//自身のクローン作成(親子関係はリセットされる)
//			std::shared_ptr<GameObject> Clone()const;

			//更新(マイフレーム呼ばれる)[呼び出し元:GameObjectUpdateProcess::Update]
			void Update(){
				_update_component();	
			};
			class GameObjectManagementProxy; //管理用関数使用仲介クラス
			//システムに呼ばれる関数群
			//初期化(作成時に呼ばれる)[呼び出し元:GameObjectManager::Resist]
			bool _Initialize(int id) { _id = id; return  _initialize_component(); };
			//シーン登録時に呼ばれる[呼び出し元:GameObjectManager::Activate]
			bool _Activated() { return _activate_component(); }
			//シーン登録解除時に呼ばれる[呼び出し元:GameObjectManager::InActivate]
			bool _InActivated() { return _inactivate_component(); }
			//終了処理(破棄時に呼ばれる)[呼び出し元:GameObjectManager::Remove]
			bool _Finalize()throw() { return _finalize_component(); };

			/*Rootとなるトランスフォームコンポーネントを取得*/
			static std::shared_ptr<components::TransformComponent> GetRootTransformComponent(bool recreate_flag = false);
		protected:
			std::shared_ptr<GameObject> me(){ return _me.lock(); } //自分のスマポ取得
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

			utility::WeakPointer<core::ISceneAccessForGameObject> _scene;

			std::weak_ptr<GameObject> _me; //自分のスマートポインタ
			std::unordered_map<int,std::shared_ptr<Component>> _component_list; //コンポーネントリスト
			const std::unordered_map<int, std::shared_ptr<Component>>& _GetComponentList()const override { return _component_list; }
			std::vector<std::shared_ptr<Component>> _component_update_list; //コンポーネント更新リスト

			//////////特殊コンポーネント//////////
			std::shared_ptr<components::TransformComponent> _transform; //ローカル形状情報
			utility::WeakPointer<components::GroundComponent> _belonging_ground; //所属地形(コンストラクタでダミーがセットされる)

			bool _is_active;
			int _id; //ゲームオブジェクトID(-1で未登録)

			bool _initialize_component();
			void _update_component();
			bool _finalize_component();
			bool _activate_component();
			bool _inactivate_component();
			template<class C>
			utility::WeakPointer<C> _add_component(){ 
				static_assert(std::is_base_of<Component, C>::value == true, "C is not derived Component.");
				auto ptr = C::MakeShared<C>();
				ptr->SetGameObject(me(), _component_id_counter);
				_component_list.insert(std::make_pair(_component_id_counter++, ptr));
				//更新処理を行う場合は更新リストに登録
				std::shared_ptr<Component> com_ptr = ptr;
				if (!com_ptr->NoUpdate_()) { _component_update_list.push_back(ptr); }
				return ptr;
			}
			int _component_id_counter;
			std::unique_ptr<GameObjectSetUpProxy> _game_object_set_up_proxy;
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

#endif