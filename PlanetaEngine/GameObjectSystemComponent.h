﻿#pragma once

#include "GameObjectComponent.h"

namespace planeta {
	namespace core {
		class GameObjectSystemComponent : public GameObjectComponent {
		public:
			using Super = GameObjectComponent;
			/*シーンデータをセットして、子クラスが参照できるようにし、オーバーライドできないようにする。*/
			void SetSceneData(const util::WeakPointer<core::SceneData>& scene_data)override final { scene_data_ = scene_data; }
			virtual ~GameObjectSystemComponent() = default;
		protected:
			core::SceneData& scene_data_ref() { return *scene_data_; }
			const core::SceneData& scene_data_ref()const { return *scene_data_; }
		private:
			util::WeakPointer<core::SceneData> scene_data_;
		};
		PE_REFLECTABLE_CLASS(GameObjectSystemComponent);
	}
}
