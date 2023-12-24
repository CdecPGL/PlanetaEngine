#pragma once

#include "planeta/PlanetaEngine.hpp"

class CDestroyerByAltitude final: public plnt::GameObjectStandardComponent {
public:
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDestroyerByAltitude);
	using Super = plnt::GameObjectStandardComponent;
	double altitude_limit()const { return altitude_limit_; }
	void altitude_limit(double l) { altitude_limit_ = l; }
private:
	double altitude_limit_= 0.0;
	plnt::NonOwingPointer<plnt::CTransform2D> my_c_transform2d_;
	virtual bool GetOtherComponentsProc(const plnt::GOComponentGetter&) override;
	virtual void OnInitialized() override;
};
PE_GAMEOBJECTCOMPONENT_CLASS(CDestroyerByAltitude);