#pragma once

#include "planeta/Task.hpp"
#include "GameObjectComponent.hpp"

namespace plnt {
	namespace private_ {
		class GameObjectCoponentUpdateProcess : public game::Task{
		public:
			using GComUpdateFuncType = void(GameObjectComponent::*)();
			using Task::Task;
			void SetGameObjectComponent(GameObjectComponent* gcom_ptr, GComUpdateFuncType gcom_update_func) {
				gcom_ptr_ = gcom_ptr;
				component_update_func_ = gcom_update_func;
			}
		private:
			void Update()override { (gcom_ptr_->*component_update_func_)(); }
			GameObjectComponent* gcom_ptr_;
			GComUpdateFuncType component_update_func_;
		};
	}
}