#pragma once

#include <unordered_map>

#include "planeta/core/signal.hpp"
#include "../core/game_object_system_component.hpp"
#include "../core/vector_2d.hpp"

namespace plnt {
	class c_ground_2d;
	class i_game_object;

	/*! 座標系*/
	enum class coordinate_system { global, ground };

	/*! 形状情報コンポーネント*/
	class c_transform_2d final : public private_::game_object_system_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_transform_2d);

	public:
		using super = game_object_system_component;
		c_transform_2d();
		~c_transform_2d() override;

		/*! グローバル空間で加速する*/
		void accelerate(const vector_2dd &acceleration);
		/*! 地面基準で加速する*/
		void ground_accelerate(const vector_2dd &ground_acceleration);
		/*! 現在の地面基準位置から移動*/
		void ground_move(const vector_2dd &mov_pos) const;
		/*! 地面基準位置の設定*/
		void ground_offset(const vector_2dd &base_ground_pos, const vector_2dd &offset) const;
		/*アクセサ*/
		//! グローバル位置を取得
		[[nodiscard]] const vector_2dd &position() const;
		//! グローバル位置を設定
		c_transform_2d &position(const vector_2dd &pos);
		//! グローバルスケールを取得
		[[nodiscard]] const vector_2dd &scale() const;
		//! グローバルスケールを設定
		c_transform_2d &scale(const vector_2dd &s);
		//! グローバル回転度を取得
		[[nodiscard]] double rotation_rad() const;
		//! グローバル回転度を設定
		c_transform_2d &rotation_rad(double rota_rad);

		//! 地形位置を取得
		[[nodiscard]] const vector_2dd &ground_position() const;
		//! 地形位置を設定
		c_transform_2d &ground_position(const vector_2dd &pos);
		//! 地形回転度を取得
		[[nodiscard]] double ground_rotation_rad() const;
		//! 地形回転度を設定
		c_transform_2d &ground_rotation_rad(double rota_rad);

		//! グローバル速度を取得
		[[nodiscard]] const vector_2dd &velocity() const;
		//! グローバル速度を設定
		c_transform_2d &velocity(const vector_2dd &vel);
		//! グローバル角速度を取得
		[[nodiscard]] double rotation_velocity_rad() const;
		//! グローバル角速度を設定
		c_transform_2d &rotation_velocity_rad(double rota_vel_rad);

		//! 地形速度を取得
		[[nodiscard]] const vector_2dd &ground_velocity() const;
		//! 地形速度を設定
		c_transform_2d &ground_velocity(const vector_2dd &vel);

		//! 速度空間を設定
		void velocity_space(coordinate_system space);
		//! 速度空間を取得
		[[nodiscard]] coordinate_system velocity_space() const;

		/*地形関係*/
		/*! 所属している地面を取得する(const版)*/
		[[nodiscard]] const c_ground_2d &ground() const;
		/*! 所属している地面を取得する*/
		c_ground_2d &ground();
		/*! 所属している地面コンポーネントを取得する*/
		[[nodiscard]] weak_pointer<c_ground_2d> get_ground() const;
		/*! 地形をセット(新しい地形、グローバル座標を維持するかどうか[true:グローバル座標が維持される,false:地形座標が維持される])*/
		bool set_ground(const weak_pointer<i_game_object> &g, bool keep_global_position);

		/*イベント*/
		/*! トランスフォーム更新イベント*/
		signal<void()> updated;

		//システム
		void apply_velocity(); //速度適用
	private:
		//実装
		class impl;
		std::unique_ptr<impl> impl_;
		//GOCオーバーライド
		void on_initialized() final;
		void on_finalized() noexcept final;
		void on_activated() final;
		void on_inactivated() final;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_transform_2d);

	namespace reflection {
		//ReflectionシステムのPtree読み込みを有効にするための定義
		inline void reflective_ptree_converter(coordinate_system &dst, const boost::property_tree::ptree &src) {
			try {
				if (const std::string str = src.get_value<std::string>(); str ==
					"ground") { dst = coordinate_system::ground; } else if (str == "global") {
					dst = coordinate_system::global;
				} else {
					throw reflection_error(util::convert_and_connect_to_string(
						"\"", src.get_value<std::string>(), "\"は\"", typeid(coordinate_system).name(),
						"\"のメンバーではありません。"));
				}
			} catch (boost::property_tree::ptree_bad_data &e) { throw reflection_error(e.what()); }
		}
	}
}
