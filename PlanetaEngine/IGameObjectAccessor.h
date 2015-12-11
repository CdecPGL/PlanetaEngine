#pragma once

#include <memory>
#include <unordered_map>
#include "WeakPointer.h"

namespace planeta_engine {
	class GameObjectComponent;
	namespace components {
		class GroundComponent;
		class TransformComponent;
	}
	namespace game {
		class IGameObjectManagerAccessor;
		class IGameProcessManagerAccessor;
		class IGameObjectAccessor {
		public:
			virtual ~IGameObjectAccessor() = 0 {};
			/*ゲームオブジェクトを有効化*/
			virtual bool Activate() = 0;
			/*ゲームオブジェクトを無効化*/
			virtual bool InActivate() = 0;
			/*ゲームオブジェクトが有効か*/
			virtual bool is_active()const = 0;
			/*ゲームオブジェクトを破棄*/
			virtual void Dispose() = 0;
			/*型でコンポーネントを取得(仮想テンプレート関数は定義できないためここで実装する)*/
			template<class C>
			utility::WeakPointer<C> GetComponent()const {
				static_assert(std::is_base_of<GameObjectComponent, C>::value == true, "C is not derived Component.");
				const auto& cl = GetComponentList_();
				for (const auto& com : cl) {
					auto ptr = std::dynamic_pointer_cast<C>(com.second);
					if (ptr != nullptr) { return ptr; }
				}
				return nullptr;
			}
			/*IDでコンポーネントを取得*/
			virtual utility::WeakPointer<GameObjectComponent> GetComponent(int id) = 0;
			/*所属地形コンポーネントを取得*/
			virtual utility::WeakPointer<components::GroundComponent> GetBelongingGroundComponent() = 0;
			/*所属地形を取得(セットされていない場合はダミーが返される)*/
			virtual const components::GroundComponent& belonging_ground()const = 0;
			virtual components::GroundComponent& belonging_ground() = 0;
			/*トランスフォームコンポーネントを取得*/
			virtual utility::WeakPointer<components::TransformComponent> GetTransformComponent() = 0;
			/*トランスフォームを取得*/
			virtual const components::TransformComponent& transform()const = 0;
			virtual components::TransformComponent& transform() = 0;
			/*所属地形をセット*/
			virtual void SetBelongingGround(const utility::WeakPointer<components::GroundComponent>& belonging_ground) = 0;
			/*弱参照を取得*/
			virtual utility::WeakPointer<IGameObjectAccessor> GetWeakPointer() = 0;
		private:
			virtual const std::unordered_map<int, std::shared_ptr<GameObjectComponent>>& GetComponentList_()const = 0;
		};
	}
}
