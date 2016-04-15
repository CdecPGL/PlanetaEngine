#pragma once

#include <unordered_map>
#include "GameObjectNormalComponent.h"
#include "Vector2D.h"
#include "WeakPointerDelegate.h"

namespace planeta_engine {
	class TransformCore;
	namespace components {
		class GroundComponent;
		/*形状情報コンポーネント*/
		class TransformComponent : public GameObjectNormalComponent {
		public:
			TransformComponent();
			~TransformComponent();

			/*現在位置から移動*/
			void Move(const Vector2Dd& mov_pos);
			/*位置の設定*/
			void Offset(const Vector2Dd& base_pos, const Vector2Dd& offset);
			/*加速する*/
			void Accelerate(const Vector2Dd& acceleration);
			/*グローバル方向に加速する*/
			void GlobalAccelerate(const Vector2Dd& global_acceleration);
			/*アクセサ*/
			const Vector2Dd& position()const;
			TransformComponent& position(const Vector2Dd& pos);
			const Vector2Dd& scale()const;
			TransformComponent&  scale(const Vector2Dd& s);
			const double rotation_rad()const;
			TransformComponent&  rotation_rad(double rota_rad);

			const Vector2Dd& global_position()const;
			TransformComponent&  global_position(const Vector2Dd& pos);
			const double global_rotation_rad()const;
			TransformComponent&  global_rotation_rad(double rota_rad);

			const Vector2Dd& velocity()const;
			TransformComponent&  velocity(const Vector2Dd& vel);
			const double rotation_velocity_rad()const;
			TransformComponent&  rotation_velocity_rad(double rota_vel_rad);

			const Vector2Dd& global_velocity()const;
			TransformComponent&  global_velocity(const Vector2Dd& vel);

			/*地形関係*/
			const GroundComponent& ground()const;
			GroundComponent& ground();
			/*地形をセット(新しい地形、グローバル座標を維持するかどうか[true:グローバル座標が維持される,false:地形座標が維持される])*/
			utility::WeakPointer<GroundComponent> GetGround()const;
			void SetGround(const utility::WeakPointer<GroundComponent>& g,bool keep_global_position);

			//システム
			void ApplyVelocity();
		private:
			bool is_no_update()const override{ return true; }

			std::unique_ptr<TransformCore> core_;
		};
	}
}
