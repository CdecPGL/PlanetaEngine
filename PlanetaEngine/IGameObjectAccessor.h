#pragma once

#include <memory>
#include <unordered_map>
#include "WeakPointer.h"
#include "WeakPointerDelegate.h"

namespace planeta_engine {
	class GameObjectComponent;
	namespace components {
		class GroundComponent;
		class TransformComponent;
	}
	namespace game {
		class IGameObjectManagerAccessor;
		class ITaskManagerAccessor;
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
			/*型でコンポーネントを取得*/
			template<class C>
			utility::WeakPointer<C> GetComponent()const {
				static_assert(std::is_base_of<GameObjectComponent, C>::value == true, "C is not derived GameComponent.");
				auto ptr = GetComponentByTypeInfo(typeid(C), [](GameObjectComponent* com) {return dynamic_cast<C*>(com) != nullptr; });
				assert(ptr == nullptr || std::dynamic_pointer_cast<C>(ptr) != nullptr);
				return ptr ? std::static_pointer_cast<C>(ptr) : nullptr;
			}
			/*型でコンポーネントを全て取得*/
			template<class C>
			std::vector<utility::WeakPointer<C>> GetAllComponents()const {
				static_assert(std::is_base_of<GameObjectComponent, C>::value == true, "C is not derived GameComponent.");
				const auto& go_list = GetAllComponentsByTypeInfo(typeid(C), [](GameObjectComponent* com) {return dynamic_cast<C*>(com) != nullptr; });
				std::vector<utility::WeakPointer<C>> ret_list;
				for (const auto& go : go_list) {
					assert(dynamic_pointer_cast<C>(go) != nullptr);
					ret_list.push_back(static_pointer_cast<C>(go));
				}
				return std::move(ret_list);
			}
			/*IDでコンポーネントを取得*/
			virtual utility::WeakPointer<GameObjectComponent> GetComponent(int id) = 0;
			/*トランスフォームコンポーネントを取得*/
			virtual utility::WeakPointer<components::TransformComponent> GetTransformComponent() = 0;
			/*トランスフォームを取得*/
			virtual const components::TransformComponent& transform()const = 0;
			virtual components::TransformComponent& transform() = 0;
			/*弱参照を取得*/
			virtual utility::WeakPointer<IGameObjectAccessor> GetWeakPointer() = 0;
			/*破棄時イベント*/
			utility::WeakPointerDelegate<void> disposed_event;
			/*アクティベート時イベント*/
			utility::WeakPointerDelegate<void> activated_event;
			/*インアクティベート時イベント*/
			utility::WeakPointerDelegate<void> inactivated_event;
		private:
			virtual std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker)const = 0;
			virtual const std::vector<std::shared_ptr<GameObjectComponent>>& GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker)const = 0;
		};
	}
}
