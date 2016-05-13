#pragma once

#include <unordered_map>
#include "GameObjectSystemComponent.h"
#include "Vector2D.h"
#include "Delegate.h"

namespace planeta_engine {
	class CGround2D;
	class IGameObject;
	/*形状情報コンポーネント*/
	class CTransform2D : public core::GameObjectSystemComponent {
	public:
		CTransform2D();
		~CTransform2D();

		/*グローバル加速する*/
		void Accelerate(const Vector2Dd& acceleration);
		/*地形加速する*/
		void GroundAccelerate(const Vector2Dd& ground_acceleration);
		/*現在地形位置から移動*/
		void GroundMove(const Vector2Dd& mov_pos);
		/*地形位置の設定*/
		void GroundOffset(const Vector2Dd& base_ground_pos, const Vector2Dd& offset);
		/*アクセサ*/
		//グローバル位置を取得
		const Vector2Dd& position()const;
		//グローバル位置を設定
		CTransform2D& position(const Vector2Dd& pos);
		//グローバルスケールを取得
		const Vector2Dd& scale()const;
		//グローバルスケールを設定
		CTransform2D&  scale(const Vector2Dd& s);
		//グローバル回転度を取得
		const double rotation_rad()const;
		//グローバル回転度を設定
		CTransform2D&  rotation_rad(double rota_rad);

		//地形位置を取得
		const Vector2Dd& ground_position()const;
		//地形位置を設定
		CTransform2D& ground_position(const Vector2Dd& pos);
		//地形回転度を取得
		const double ground_rotation_rad()const;
		//地形回転度を設定
		CTransform2D&  ground_rotation_rad(double rota_rad);

		//グローバル速度を取得
		const Vector2Dd& velocity()const;
		//グローバル速度を設定
		CTransform2D&  velocity(const Vector2Dd& vel);
		//グローバル角速度を取得
		const double rotation_velocity_rad()const;
		//グローバル角速度を設定
		CTransform2D&  rotation_velocity_rad(double rota_vel_rad);

		//地形速度を取得
		const Vector2Dd& ground_velocity()const;
		//地形速度を設定
		CTransform2D&  ground_velocity(const Vector2Dd& vel);

		//速度空間を設定
		enum class Space { Ground, Global };
		void velocity_space(Space space);

		/*地形関係*/
		const CGround2D& ground()const;
		CGround2D& ground();
		/*地形をセット(新しい地形、グローバル座標を維持するかどうか[true:グローバル座標が維持される,false:地形座標が維持される])*/
		utility::WeakPointer<CGround2D> GetGround()const;
		bool SetGround(const utility::WeakPointer<IGameObject>& g, bool keep_global_position);
		/*トランスフォーム更新イベントハンドラ追加*/
		utility::DelegateConnection AddUpdatedEventHandler(utility::DelegateHandlerAdder<void>&& handler_adder);

		//システム
		void ApplyVelocity_(); //速度適用
	private:
		//実装
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		//GOCオーバーライド
		bool OnInitialized()override final;
		void OnFinalized()noexcept override final;
		bool OnActivated()override final;
		bool OnInactivated()override final;
	};
}
