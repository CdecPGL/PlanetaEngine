#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include"Object.h"
#include "MakeSharedGameObject.h"
#include "IGameObjectAccessor.h"
#include "GameObjectManagerConnection.h"
#include "NonCopyable.h"

namespace planeta_engine {
	class GameObjectComponent;
	namespace components {
		class GroundComponent;
		class TransformComponent;
	}
	class SceneAccessorForGameObject;
	namespace game {
		class GameObjectAccessorForSetUp; //初期化用関数使用仲介クラス
		//GameObjectクラス(継承禁止ぃ)
		class GameObject final : public core::Object, public IGameObjectAccessor
		,private utility::NonCopyable<GameObject>{
			friend GameObjectAccessorForSetUp;
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
			utility::WeakPointer<GameObjectComponent> GetComponent(int id) override {
				auto it = component_list_.find(id);
				if (it == component_list_.end()) { return nullptr; } else { return it->second; }
			}
			/*ローカルトランスフォーム取得*/
			utility::WeakPointer<components::TransformComponent> GetTransformComponent()override { return transform_; }
			const components::TransformComponent& transform()const override { return *transform_; }
			components::TransformComponent& transform()override { return *transform_; }

			////////////システム関数//////////
			//作成
			static std::shared_ptr<GameObject> Create();

			/*シーンのポインタをセット*/
			void SetSceneAccessor(const utility::WeakPointer<SceneAccessorForGameObject>& scene) { scene_accessor_ = scene; }
			//自身のクローン作成(親子関係はリセットされる)
	//			std::shared_ptr<GameObject> Clone()const;

				//更新(マイフレーム呼ばれる)[呼び出し元:GameObjectUpdateProcess::Update]
			void Update() {
				UpdateComponent_();
			};
			class GameObjectManagementProxy; //管理用関数使用仲介クラス
			void SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mc) { manager_connection_ = std::move(mc); }
			//初期化関連関数[呼び出し元:GameObjectSetUpper]
			bool SetUpSystemComponent();
			bool Initialize_() { return  InitializeComponent_(); };
			//システムに呼ばれる関数群
			//シーン登録時に呼ばれる[呼び出し元:GameObjectManager::Activate]
			bool Activated_() { activated_event(); return ActivateComponent_(); }
			//シーン登録解除時に呼ばれる[呼び出し元:GameObjectManager::InActivate]
			bool InActivated_() { inactivated_event(); return InactivateComponent_(); }
			//終了処理(破棄時に呼ばれる)[呼び出し元:GameObjectManager::Remove]
			bool Finalize_()throw() { disposed_event(); return FinalizeComponent_(); };

			/*弱参照を取得*/
			utility::WeakPointer<IGameObjectAccessor> GetWeakPointer()override { return me(); };
		protected:
			std::shared_ptr<GameObject> me() { return me_.lock(); } //自分のスマポ取得
		private:
			//直接生成、コピーの禁止
			//既定のコンストラクタ
			GameObject();
			~GameObject();

			//			static void* operator new(size_t s){ return Object::operator new(s); }
			//			static void operator delete(void* p){ return Object::operator delete(p); }
			//			static void* operator new[](size_t s){ return Object::operator new[](s); }
			//			static void operator delete[](void* p){ return Object::operator delete[](p); }

			utility::WeakPointer<SceneAccessorForGameObject> scene_accessor_;
			std::unique_ptr<GameObjectManagerConnection> manager_connection_;

			std::weak_ptr<GameObject> me_; //自分のスマートポインタ
			std::unordered_map<int, std::shared_ptr<GameObjectComponent>> component_list_; //コンポーネントリスト
			std::vector<std::shared_ptr<GameObjectComponent>> component_update_list_; //コンポーネント更新リスト

			mutable std::unordered_map<std::type_index, std::pair<bool, std::vector<std::shared_ptr<GameObjectComponent>>>> component_type_map_; //タイプによるコンポーネントマップ<typeindex,<完全探索済みか(false:少なくとも１つは探索済み,true:全て探索済み),コンポーネントリスト>>
			void AddComponentToTypeInfoMap(const std::type_info& ti,const std::shared_ptr<GameObjectComponent>& com);
			std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker)const override;
			const std::vector<std::shared_ptr<GameObjectComponent>>& GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker)const override;


			//////////特殊コンポーネント//////////
			std::shared_ptr<components::TransformComponent> transform_; //ローカル形状情報

			bool is_active_;
			/*ゲームオブジェクトコンポーネントに対する一括処理*/
			bool InitializeComponent_();
			void UpdateComponent_();
			bool FinalizeComponent_();
			bool ActivateComponent_();
			bool InactivateComponent_();
			/*ゲームオブジェクトコンポーネントを追加する*/
			template<class C>
			utility::WeakPointer<C> add_component_() {
				static_assert(std::is_base_of<GameObjectComponent, C>::value == true, "C is not derived Component.");
				auto ptr = C::MakeShared<C>();
				SystemSetUpComponent_(*ptr);
				component_list_.insert(std::make_pair(component_id_counter_++, ptr));
				//更新処理を行う場合は更新リストに登録
				std::shared_ptr<GameObjectComponent> com_ptr = ptr;
				if (!com_ptr->is_no_update()) { component_update_list_.push_back(ptr); }
				//タイプリストに追加する
				AddComponentToTypeInfoMap(typeid(C), com_ptr);
				return ptr;
			}
			/*コンポーネントのシステム設定を行う*/
			void SystemSetUpComponent_(GameObjectComponent& com);
			int component_id_counter_;
			struct create_helper;
		};
		struct GameObject::create_helper {
			GameObject x;
			// クラス内クラスだとテンプレートにできるのでテンプレートで書いてみる
			template<class... Args>
			explicit create_helper(Args&&... args)
				: x(std::forward<Args>(args)...) {}
			~create_helper() {}
		};
	}
}
