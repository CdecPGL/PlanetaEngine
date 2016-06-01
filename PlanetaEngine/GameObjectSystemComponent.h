#pragma once

#include "GameObjectComponent.h"

namespace planeta_engine {
	namespace core {
		class GameObjectSystemComponent : public GameObjectComponent {
		public:
			/*�V�[���f�[�^���Z�b�g���āA�q�N���X���Q�Ƃł���悤�ɂ��A�I�[�o�[���C�h�ł��Ȃ��悤�ɂ���B*/
			void SetSceneData(const util::WeakPointer<core::SceneData>& scene_data)override final { scene_data_ = scene_data; }
			virtual ~GameObjectSystemComponent() = default;
		protected:
			core::SceneData& scene_data_ref() { return *scene_data_; }
			const core::SceneData& scene_data_ref()const { return *scene_data_; }
		private:
			util::WeakPointer<core::SceneData> scene_data_;
		};
	}
}
