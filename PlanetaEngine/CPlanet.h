#pragma once
#include "GameObjectStandardComponent.h"
#include <vector>

namespace planeta {
	class CPlanet : public GameObjectStandardComponent {
	public:
		CPlanet();
		~CPlanet() = default;
		void SetHeightByIndex(unsigned int idx, double h) { SetGapByIndex(idx, h - _radius); }
		double GetHeightByRad(double rad)const { return _radius + GetGapByRad(rad); }

		/*アクセサ*/
		CPlanet& radius(double r) { _radius = r; return *this; }
		double radius()const { return _radius; }
		CPlanet& separation(unsigned int s);
		unsigned int separation()const { return _separation; }
	private:
		void SetGapByIndex(unsigned int idx, double gap);
		double GetGapByRad(double rad)const;
		double _radius = 100;
		unsigned int _separation;
		std::vector<double> _gap;
	};
	PE_REGISTER_GAMEOBJECTCOMPONENT(CPlanet);
}