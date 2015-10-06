#pragma once
#include "Component.h"
#include <vector>

namespace planeta_engine {
	namespace components {
		class PlanetComponent : public game::Component {
		public:
			PlanetComponent();
			~PlanetComponent() = default;
			void SetHeightByIndex(unsigned int idx, double h) { SetGapByIndex(idx, h - _radius); }
			double GetHeightByRad(double rad)const { return _radius + GetGapByRad(rad); }

			/*アクセサ*/
			void radius(double r) { _radius = r; }
			double radius()const { return _radius; }
			void separation(unsigned int s);
			unsigned int separation()const { return _separation; }
		private:
			void SetGapByIndex(unsigned int idx, double gap);
			double GetGapByRad(double rad)const;
			double _radius;
			unsigned int _separation;
			std::vector<double> _gap;
		};
	}
}