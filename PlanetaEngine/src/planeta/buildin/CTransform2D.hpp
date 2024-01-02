#pragma once

#include <unordered_map>

#include "planeta/core/signal.hpp"
#include "..\core\game_object_system_component.hpp"
#include "..\core\vector_2d.hpp"

namespace plnt {
	class CGround2D;
	class i_game_object;

	/*! 座標系*/
	enum class CoordinateSystem { Global, Ground };

	/*! 形状情報コンポーネント*/
	class CTransform2D : public private_::game_object_system_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CTransform2D);

	public:
		using super = private_::game_object_system_component;
		CTransform2D();
		~CTransform2D();

		/*! グローバル空間で加速する*/
		void Accelerate(const vector_2dd &acceleration);
		/*! 地面基準で加速する*/
		void GroundAccelerate(const vector_2dd &ground_acceleration);
		/*! 現在の地面基準位置から移動*/
		void GroundMove(const vector_2dd &mov_pos);
		/*! 地面基準位置の設定*/
		void GroundOffset(const vector_2dd &base_ground_pos, const vector_2dd &offset);
		/*アクセサ*/
		//! グローバル位置を取得
		const vector_2dd &position() const;
		//! グローバル位置を設定
		CTransform2D &position(const vector_2dd &pos);
		//! グローバルスケールを取得
		const vector_2dd &scale() const;
		//! グローバルスケールを設定
		CTransform2D &scale(const vector_2dd &s);
		//! グローバル回転度を取得
		const double rotation_rad() const;
		//! グローバル回転度を設定
		CTransform2D &rotation_rad(double rota_rad);

		//! 地形位置を取得
		const vector_2dd &ground_position() const;
		//! 地形位置を設定
		CTransform2D &ground_position(const vector_2dd &pos);
		//! 地形回転度を取得
		const double ground_rotation_rad() const;
		//! 地形回転度を設定
		CTransform2D &ground_rotation_rad(double rota_rad);

		//! グローバル速度を取得
		const vector_2dd &velocity() const;
		//! グローバル速度を設定
		CTransform2D &velocity(const vector_2dd &vel);
		//! グローバル角速度を取得
		const double rotation_velocity_rad() const;
		//! グローバル角速度を設定
		CTransform2D &rotation_velocity_rad(double rota_vel_rad);

		//! 地形速度を取得
		const vector_2dd &ground_velocity() const;
		//! 地形速度を設定
		CTransform2D &ground_velocity(const vector_2dd &vel);

		//! 速度空間を設定
		void velocity_space(CoordinateSystem space);
		//! 速度空間を取得
		CoordinateSystem velocity_space() const;

		/*地形関係*/
		/*! 所属している地面を取得する(const版)*/
		const CGround2D &ground() const;
		/*! 所属している地面を取得する*/
		CGround2D &ground();
		/*! 所属している地面コンポーネントを取得する*/
		weak_pointer<CGround2D> GetGround() const;
		/*! 地形をセット(新しい地形、グローバル座標を維持するかどうか[true:グローバル座標が維持される,false:地形座標が維持される])*/
		bool SetGround(const weak_pointer<i_game_object> &g, bool keep_global_position);

		/*イベント*/
		/*! トランスフォーム更新イベント*/
		signal<void()> updated;

		//システム
		void ApplyVelocity_(); //速度適用
	private:
		//実装
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		//GOCオーバーライド
		void on_initialized() override final;
		void on_finalized() noexcept override final;
		void on_activated() override final;
		void on_inactivated() override final;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CTransform2D);

	namespace reflection {
		//ReflectionシステムのPtree読み込みを有効にするための定義
		inline void reflective_ptree_converter(::plnt::CoordinateSystem &dst, const boost::property_tree::ptree &src) {
			try {
				std::string str = src.get_value<std::string>();
				if (str == "Ground") { dst = ::plnt::CoordinateSystem::Ground; } else if (str == "Global") {
					dst = ::plnt::CoordinateSystem::Global;
				} else {
					throw ::plnt::reflection::reflection_error(util::ConvertAndConnectToString(
						"\"", src.get_value<std::string>(), "\"は\"", typeid(::plnt::CoordinateSystem).name(),
						"\"のメンバーではありません。"));
				}
			} catch (boost::property_tree::ptree_bad_data &e) { throw ::plnt::reflection::reflection_error(e.what()); }
		}
	}
}
