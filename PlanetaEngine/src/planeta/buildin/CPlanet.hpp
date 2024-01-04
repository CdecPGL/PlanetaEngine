#pragma once

#include <vector>

#include "../core/game_object_standard_component.hpp"

namespace plnt {
	/*! 平面惑星コンポーネント*/
	class c_planet final : public game_object_standard_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_planet);

	public:
		using super = game_object_standard_component;
		c_planet();
		~c_planet() override = default;
		/*! 地表の高さを頂点インデックスで設定するb*/
		void set_height_by_index(const unsigned int idx, const double h) { set_gap_by_index(idx, h - radius_); }
		/*! 地表の高さを角度で取得*/
		[[nodiscard]] double get_height_by_rad(const double rad) const { return radius_ + get_gap_by_rad(rad); }

		/*アクセサ*/
		/*! 惑星の半径取得*/
		c_planet &radius(const double r) {
			radius_ = r;
			return *this;
		}

		/*! 惑星の半径設定*/
		[[nodiscard]] double radius() const { return radius_; }
		/*! 分割数取得*/
		c_planet &separation(unsigned int s);
		/*! 分割数設定*/
		[[nodiscard]] unsigned int separation() const { return separation_; }

	private:
		void set_gap_by_index(unsigned int idx, double gap);
		double get_gap_by_rad(double rad) const;
		double radius_ = 100;
		unsigned int separation_;
		std::vector<double> gap_;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_planet);
}
