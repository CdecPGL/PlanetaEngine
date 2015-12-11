#pragma once

#include <unordered_map>
#include "GameObjectNormalComponent.h"
#include "Vector2D.h"
//#include "Matrix.h"

namespace planeta_engine {
	namespace components {
		/*形状情報コンポーネント*/
		class TransformComponent : public GameObjectNormalComponent {
		public:
			TransformComponent();
			~TransformComponent() = default;
			void AddChild(const utility::WeakPointer<TransformComponent>& c);
			void RemoveChild(const utility::WeakPointer<TransformComponent>& c);
			/*アクセサ*/
			const Vector2D<double>& local_position()const { return local_position_; }
			void local_position(const Vector2D<double>& pos) { local_position_ = pos; ConvertLocalToGlobal(); }
			const Vector2D<double>& local_scale()const { return local_scale_; }
			void local_scale(const Vector2D<double>& s) { local_scale_ = s; ConvertLocalToGlobal();}
			const double local_rotation_rad()const { return local_rotation_rad_; }
			void local_rotation_rad(double rota_rad) { local_rotation_rad_ = rota_rad; ConvertLocalToGlobal(); }
			const Vector2D<double>& velocity()const { return _velocity; }
			void velocity(const Vector2D<double>& vel) { _velocity = vel; }
			const double rotation_velosity_rad()const { return _rotation_velocity_rad; }
			void rotation_velocity_rad(double rota_vel_rad) { _rotation_velocity_rad = rota_vel_rad; }
			utility::WeakPointer<TransformComponent> parent()const { return parent_; }
			void parent(const utility::WeakPointer<TransformComponent>& p);
			const Vector2D<double>& global_position()const { return global_position_; }
			void global_position(const Vector2D<double>& pos) { global_position_ = pos; ConvertGlobalToLocal(); }
			const Vector2D<double>& global_scale()const { return global_scale_; }
			void global_scale(const Vector2D<double>& s) { global_scale_ = s; ConvertGlobalToLocal(); }
			const double global_rotation_rad()const { return global_rotation_rad_; }
			void global_rotation_rad(double rota_rad) { global_rotation_rad_ = rota_rad; ConvertGlobalToLocal(); }
			void position_parent_dependence(bool f) { position_parent_dependence_ = f; }
			void rotation_parent_dependence(bool f) { rotation_parent_dependence_ = f; }
			void scale_parent_dependence(bool f) { scale_parent_dependence_ = f; }
			//システム
			void ApplyVelocity();
			void ApplyVelocityRecursively();
		private:
			bool is_no_update()const override{ return true; }

			void ConvertLocalToGlobal();
			void ConvertGlobalToLocal();
			void ConvertLocalToGlobalChildren();

			utility::WeakPointer<TransformComponent> root_transform();

			//形状情報
			Vector2D<double> local_position_;
			Vector2D<double> global_position_;
			Vector2D<double> local_scale_ = Vector2D<double>(1.0, 1.0);
			Vector2D<double> global_scale_ = Vector2D<double>(1.0, 1.0);
			double local_rotation_rad_ = 0.0;
			double global_rotation_rad_ = 0.0;
			//物理情報
			Vector2D<double> _velocity;
			double _rotation_velocity_rad = 0.0;
			//親子関係
			utility::WeakPointer<TransformComponent> parent_;
			std::unordered_map<void*, utility::WeakPointer<TransformComponent>> children_;
			bool position_parent_dependence_ = true;
			bool scale_parent_dependence_ = true;
			bool rotation_parent_dependence_ = true;
			//グローバル変換行列
			/*math::Matrix<double, 3, 3> global_matrix_;
			const math::Matrix<double, 3, 3>& global_matrix()const { return global_matrix_; }
			const math::Matrix<double, 3, 3> global_matrix_position()const {
				return std::move(math::Matrix<double, 3, 3>({ {1,0,0}, {0,1,0}, {global_matrix_[0][2],global_matrix_[1][2],1} }));
			}
			const math::Matrix<double, 3, 3> global_matrix_scale()const;
			const math::Matrix<double, 3, 3> global_matrix_rotation()const;*/

		};
	}
}
