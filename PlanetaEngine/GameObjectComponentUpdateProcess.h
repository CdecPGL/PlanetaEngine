#pragma once

#include "GameProcess.h"
#include "GameObjectComponent.h"

namespace planeta_engine {
	namespace core {
		class GameObjectCoponentUpdateProcess : public game::GameProcess{
		public:
			using GCOM_UPDATE_FUNC_TYPE = void(GameObjectComponent::*)();
			using GameProcess::GameProcess;
			void SetGameObjectComponent(GameObjectComponent* gcom_ptr, GCOM_UPDATE_FUNC_TYPE gcom_update_func) {
				gcom_ptr_ = gcom_ptr;
				component_update_func_ = gcom_update_func;
			}
		private:
			void Update()override { (gcom_ptr_->*component_update_func_)(); }
			GameObjectComponent* gcom_ptr_;
			GCOM_UPDATE_FUNC_TYPE component_update_func_;
		};
	}
}