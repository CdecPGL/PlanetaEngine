#pragma once
#include "boost/optional/optional.hpp"
#include "GameObjectStandardComponent.h"
#include "MathMatrix.h"
#include "Vector2D.h"
#include "MathConstant.h"
#include "IColliderWithCollider2D.h"

namespace planeta_engine {
	class CCircleCollider2D;
	class CStraightLineCollider2D;
	class CTransform2D;
	class CGround2D :
		public GameObjectStandardComponent, public core::IColliderWithCollider2D {
	public:
		CGround2D();
		virtual ~CGround2D() = default;
		/*指定地形座標の地面からの高さを取得*/
		virtual double GetAltitudeWithGroundPosition(const Vector2Dd& ground_pos)const = 0;
		/*グローバル座標を地形座標に変換*/
		virtual Vector2Dd ConvertPositionGlobalToGround(const Vector2Dd& global_pos)const = 0;
		/*地形座標をグローバル座標に変換*/
		virtual Vector2Dd ConvertPositionGroundToGlobal(const Vector2Dd& ground_pos)const = 0;
		/*指定地形座標でグローバル回転度を地形回転度に変換*/
		double ConvertRotationGlobalToGroundWithGroundPosition(const Vector2Dd& ground_pos, double global_rota_rad)const;
		/*指定地形座標で地形回転度をグローバル回転度に変換*/
		double ConvertRotationGroundToGlobalWithGroundPosition(const Vector2Dd& ground_pos, double ground_rota_rad)const;
		/*指定地形座標でグローバル速度を地形速度に変換*/
		Vector2Dd ConvertVelocityGlobalToGroundWithGroundPosition(const Vector2Dd& ground_pos, const Vector2Dd& global_velocity);
		/*指定地形座標で地形速度をグローバル速度に変換*/
		Vector2Dd ConvertVelocityGroundToGlobalWithGroundPosition(const Vector2Dd& ground_pos, const Vector2Dd& ground_velocity);
		/*指定地形座標で地形座標系ベクトルをグローバル座標系での長さと一致するよう変換*/
		virtual Vector2Dd NormalizeGroundVectorWithGroundPosition(const Vector2Dd& ground_pos, const Vector2Dd& ground_vector)const = 0;
		/*X座標の範囲取得*/
		//const std::pair<double,double>& GetXRange()const;
		/*Y座標の周期取得*/
		//const std::pair<double, double>& GetYRange()const;
		/*X座標の*/
	protected:
		//void SetXRange(const std::pair<double, double>& range);
		//void SetYRange(const std::pair<double, double>& range);

		utility::NonOwingPointer<CTransform2D> transform2d_;

		virtual bool OnInitialized() override;
		virtual bool OnActivated() override;
		virtual bool OnInactivated() override;
		virtual void OnFinalized() noexcept override;
	private:
		/*指定地形座標で地形座標系回転度のグローバル座標系回転度との差を取得する*/
		virtual double GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const Vector2Dd& ground_pos)const = 0;
	};
}
