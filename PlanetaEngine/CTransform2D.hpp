#pragma once

#include <unordered_map>

#include "Signal.h"
#include "GameObjectSystemComponent.h"
#include "Vector2D.h"

namespace planeta {
	class CGround2D;
	class IGameObject;
	/*! 座標系*/
	enum class CoordinateSystem { Global, Ground };
	/*! 形状情報コンポーネント*/
	class CTransform2D : public private_::GameObjectSystemComponent {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CTransform2D);
	public:
		using Super = private_::GameObjectSystemComponent;
		CTransform2D();
		~CTransform2D();

		/*! グローバル空間で加速する*/
		void Accelerate(const Vector2Dd& acceleration);
		/*! 地面基準で加速する*/
		void GroundAccelerate(const Vector2Dd& ground_acceleration);
		/*! 現在の地面基準位置から移動*/
		void GroundMove(const Vector2Dd& mov_pos);
		/*! 地面基準位置の設定*/
		void GroundOffset(const Vector2Dd& base_ground_pos, const Vector2Dd& offset);
		/*アクセサ*/
		//! グローバル位置を取得
		const Vector2Dd& position()const;
		//! グローバル位置を設定
		CTransform2D& position(const Vector2Dd& pos);
		//! グローバルスケールを取得
		const Vector2Dd& scale()const;
		//! グローバルスケールを設定
		CTransform2D&  scale(const Vector2Dd& s);
		//! グローバル回転度を取得
		const double rotation_rad()const;
		//! グローバル回転度を設定
		CTransform2D&  rotation_rad(double rota_rad);

		//! 地形位置を取得
		const Vector2Dd& ground_position()const;
		//! 地形位置を設定
		CTransform2D& ground_position(const Vector2Dd& pos);
		//! 地形回転度を取得
		const double ground_rotation_rad()const;
		//! 地形回転度を設定
		CTransform2D&  ground_rotation_rad(double rota_rad);

		//! グローバル速度を取得
		const Vector2Dd& velocity()const;
		//! グローバル速度を設定
		CTransform2D&  velocity(const Vector2Dd& vel);
		//! グローバル角速度を取得
		const double rotation_velocity_rad()const;
		//! グローバル角速度を設定
		CTransform2D&  rotation_velocity_rad(double rota_vel_rad);

		//! 地形速度を取得
		const Vector2Dd& ground_velocity()const;
		//! 地形速度を設定
		CTransform2D&  ground_velocity(const Vector2Dd& vel);

		//! 速度空間を設定
		void velocity_space(CoordinateSystem space);
		//! 速度空間を取得
		CoordinateSystem velocity_space()const;

		/*地形関係*/
		/*! 所属している地面を取得する(const版)*/
		const CGround2D& ground()const;
		/*! 所属している地面を取得する*/
		CGround2D& ground();
		/*! 所属している地面コンポーネントを取得する*/
		WeakPointer<CGround2D> GetGround()const;
		/*! 地形をセット(新しい地形、グローバル座標を維持するかどうか[true:グローバル座標が維持される,false:地形座標が維持される])*/
		bool SetGround(const WeakPointer<IGameObject>& g, bool keep_global_position);

		/*イベント*/
		/*! トランスフォーム更新イベント*/
		Signal<void()> updated;

		//システム
		void ApplyVelocity_(); //速度適用
	private:
		//実装
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		//GOCオーバーライド
		void OnInitialized()override final;
		void OnFinalized()noexcept override final;
		void OnActivated()override final;
		void OnInactivated()override final;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CTransform2D);

	namespace util {
		//ReflectionシステムのPtree読み込みを有効にするための定義
		inline void ReflectivePtreeConverter(::planeta::CoordinateSystem & dst, const boost::property_tree::ptree& src) {
			try {
				std::string str = src.get_value<std::string>();
				if (str == "Ground") {
					dst = ::planeta::CoordinateSystem::Ground;
				} else if (str == "Global") {
					dst = ::planeta::CoordinateSystem::Global;
				} else {
					throw planeta::reflection_error(util::ConvertAndConnectToString("\"", src.get_value<std::string>(), "\"は\"", typeid(::planeta::CoordinateSystem).name(), "\"のメンバーではありません。"));
				}
			} catch (boost::property_tree::ptree_bad_data& e) {
				throw planeta::reflection_error(e.what());
			}
		}
	}
}
