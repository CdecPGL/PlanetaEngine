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
			~TransformComponent() = default;

			/*現在位置から移動*/
			void Move(const Vector2D<double>& mov_pos);
			/*位置の設定*/
			void Offset(const Vector2D<double>& base_pos, const Vector2D<double>& offset);
			/*加速する*/
			void Accelerate(const Vector2D<double>& acceleration);
			/*グローバル方向に加速する*/
			void GlobalAccelerate(const Vector2D<double>& global_acceleration);
			/*アクセサ*/
			const Vector2D<double>& position()const;
			TransformComponent& position(const Vector2D<double>& pos);
			const Vector2D<double>& scale()const;
			TransformComponent&  scale(const Vector2D<double>& s);
			const double rotation_rad()const;
			TransformComponent&  rotation_rad(double rota_rad);

			const Vector2D<double>& global_position()const;
			TransformComponent&  global_position(const Vector2D<double>& pos);
			const double global_rotation_rad()const;
			TransformComponent&  global_rotation_rad(double rota_rad);

			const Vector2D<double>& velocity()const;
			TransformComponent&  velocity(const Vector2D<double>& vel);
			const double rotation_velocity_rad()const;
			TransformComponent&  rotation_velocity_rad(double rota_vel_rad);

			const Vector2D<double>& global_velocity()const;
			TransformComponent&  global_velocity(const Vector2D<double>& vel);

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

			void UpdateTransformDataGlobalByGround(); //地形座標からグローバル座標を算出
			void UpdateTransformDataGroundByGlobal(); //地形座標からローカル座標を算出
			void UpdatePhysicalDataGlobal(); //地形座標からグローバル座標を算出
			void UpdatePhysicalDataGround(); //地形座標からローカル座標を算出

			utility::WeakPointer<GroundComponent> ground_; //コンストラクタでダミーをセット
			/*ダミーの地形コンポーネントを取得*/
			static std::shared_ptr<components::GroundComponent> GetDumyGroundComponent_();

			enum class CoordinateSystem { Global, Ground ,None};
			//形状情報
			struct TransformData {
				Vector2D<double> position;
				Vector2D<double> scale = Vector2D<double>(1.0, 1.0);
				double rotation_rad = 0.0;
			};
			TransformData ground_transform_data_;
			TransformData global_transform_data_;
			CoordinateSystem position_last_update = CoordinateSystem::None;
			CoordinateSystem rotation_last_update = CoordinateSystem::None;

			//物理情報
			Vector2D<double> ground_velocity_; //地形座標系での速度。長さはグローバル座標系に準ずる
			Vector2D<double> global_velocity_;
			double rotation_velocity_rad_ = 0.0;
			CoordinateSystem velocity_last_update = CoordinateSystem::None;
		};
	}
}
