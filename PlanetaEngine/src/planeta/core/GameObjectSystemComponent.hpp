#pragma once

#include "planeta/core/GameObjectComponent.hpp"

namespace plnt {
	namespace private_ {
		class GameObjectSystemComponent : public GameObjectComponent {
		public:
			using Super = GameObjectComponent;
			/*シーンデータをセットして、子クラスが参照できるようにし、オーバーライドできないようにする。*/
			void SetSceneInternalInterface(const WeakPointer<private_::ISceneInternal>& scene_data)override final { scene_internal_interface_ = scene_data; }
			virtual ~GameObjectSystemComponent() = default;
		protected:
			private_::ISceneInternal& scene_internal_interface() { return *scene_internal_interface_; }
			const private_::ISceneInternal& scene_internal_interface()const { return *scene_internal_interface_; }
		private:
			WeakPointer<private_::ISceneInternal> scene_internal_interface_;
		};
		PE_REFLECTABLE_CLASS(GameObjectSystemComponent);
	}
}
