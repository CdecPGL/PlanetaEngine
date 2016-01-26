#pragma once

#include <unordered_map>
#include "GameObjectNormalComponent.h"
#include "Vector2D.h"
#include "WeakPointerDelegate.h"

namespace planeta_engine {
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
			/*アクセサ*/
			const Vector2D<double>& position()const;
			void position(const Vector2D<double>& pos);
			const Vector2D<double>& scale()const;
			void scale(const Vector2D<double>& s);
			const double rotation_rad()const;
			void rotation_rad(double rota_rad);

			const Vector2D<double>& global_position()const;
			void global_position(const Vector2D<double>& pos);
			const double global_rotation_rad()const;
			void global_rotation_rad(double rota_rad);

			const Vector2D<double>& velocity()const;
			void velocity(const Vector2D<double>& vel);
			const double rotation_velocity_rad()const;
			void rotation_velocity_rad(double rota_vel_rad);

			const Vector2D<double>& global_velocity()const;
			void global_velocity(const Vector2D<double>& vel);

			//システム
			void ApplyVelocity();
		private:
			bool is_no_update()const override{ return true; }

			void UpdateTransformDataGlobalByGround(); //地形座標からグローバル座標を算出
			void UpdateTransformDataGroundByGlobal(); //地形座標からローカル座標を算出
			void UpdatePhysicalDataGlobal(); //地形座標からグローバル座標を算出
			void UpdatePhysicalDataGround(); //地形座標からローカル座標を算出

			utility::WeakPointer<GroundComponent> belonging_ground_;

			//形状情報
			struct TransformData {
				Vector2D<double> position;
				Vector2D<double> scale = Vector2D<double>(1.0, 1.0);
				double rotation_rad = 0.0;
				bool position_updated = false;
				bool rotation_updated = false;
			};
			TransformData ground_transform_data_;
			TransformData global_transform_data_;

			//物理情報
			Vector2D<double> ground_velocity_; //地形座標系での速度。長さはグローバル座標系に準ずる
			bool ground_velocity_updated_ = false;
			Vector2D<double> global_velocity_;
			bool global_velocity_updated_ = false;
			double rotation_velocity_rad_;
		};
	}
}
