#pragma once
#include "boost/optional/optional.hpp"
#include "GameObjectNormalComponent.h"
#include "Matrix.h"
#include "Vector2D.h"
#include "MathConstant.h"
#include "IColliderWithCollider.h"

namespace planeta_engine {
	namespace components {
		class CircleColliderComponent;
		class StraightLineColliderComponent;
		class GroundComponent :
			public GameObjectNormalComponent , public core::IColliderWithCollider
		{
		public:
			GroundComponent();
			virtual ~GroundComponent() = default;
			/*指定地形座標の地面からの高さを取得*/
			virtual double GetAltitudeWithGroundPosition(const Vector2D<double>& ground_pos)const = 0;
			/*グローバル座標を地形座標に変換*/
			virtual Vector2D<double> ConvertPositionGlobalToGround(const Vector2D<double>& global_pos)const = 0;
			/*地形座標をグローバル座標に変換*/
			virtual Vector2D<double> ConvertPositionGroundToGlobal(const Vector2D<double>& ground_pos)const = 0;
			/*指定地形座標でグローバル回転度を地形回転度に変換*/
			double ConvertRotationGlobalToGroundWithGroundPosition(const Vector2D<double>& ground_pos, double global_rota_rad)const;
			/*指定地形座標で地形回転度をグローバル回転度に変換*/
			double ConvertRotationGroundToGlobalWithGroundPosition(const Vector2D<double>& ground_pos, double ground_rota_rad)const;
			/*指定地形座標でグローバル速度を地形速度に変換*/
			Vector2D<double> ConvertVelocityGlobalToGroundWithGroundPosition(const Vector2D<double>& ground_pos, const Vector2D<double>& global_velocity);
			/*指定地形座標で地形速度をグローバル速度に変換*/
			Vector2D<double> ConvertVelocityGroundToGlobalWithGroundPosition(const Vector2D<double>& ground_pos, const Vector2D<double>& ground_velocity);
			/*指定地形座標で地形座標系ベクトルをグローバル座標系での長さと一致するよう変換*/
			virtual Vector2D<double> NormalizeGroundVectorWithGroundPosition(const Vector2D<double>& ground_pos, const Vector2D<double>& ground_vector)const = 0;
			/*X座標の範囲取得*/
			//const std::pair<double,double>& GetXRange()const;
			/*Y座標の周期取得*/
			//const std::pair<double, double>& GetYRange()const;
			/*X座標の*/
		protected:
			//void SetXRange(const std::pair<double, double>& range);
			//void SetYRange(const std::pair<double, double>& range);

			virtual bool OnInitialized() override;
			virtual bool OnActivated() override;
			virtual void OnUpdated() override;
			virtual bool OnInactivated() override;
			virtual void OnFinalized() noexcept override;
		private:
			/*指定地形座標で地形座標系回転度のグローバル座標系回転度との差を取得する*/
			virtual double GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const Vector2D<double>& ground_pos)const = 0;
		};
	}
}
