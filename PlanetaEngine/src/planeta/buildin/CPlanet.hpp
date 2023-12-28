#pragma once

#include <vector>

#include "..\core\game_object_standard_component.hpp"

namespace plnt {
	/*! 平面惑星コンポーネント*/
	class CPlanet : public game_object_standard_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CPlanet);

	public:
		using super = game_object_standard_component;
		CPlanet();
		~CPlanet() = default;
		/*! 地表の高さを頂点インデックスで設定するb*/
		void SetHeightByIndex(unsigned int idx, double h) { SetGapByIndex(idx, h - _radius); }
		/*! 地表の高さを角度で取得*/
		double GetHeightByRad(double rad) const { return _radius + GetGapByRad(rad); }

		/*アクセサ*/
		/*! 惑星の半径取得*/
		CPlanet &radius(double r) {
			_radius = r;
			return *this;
		}

		/*! 惑星の半径設定*/
		double radius() const { return _radius; }
		/*! 分割数取得*/
		CPlanet &separation(unsigned int s);
		/*! 分割数設定*/
		unsigned int separation() const { return _separation; }

	private:
		void SetGapByIndex(unsigned int idx, double gap);
		double GetGapByRad(double rad) const;
		double _radius = 100;
		unsigned int _separation;
		std::vector<double> _gap;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CPlanet);
}
