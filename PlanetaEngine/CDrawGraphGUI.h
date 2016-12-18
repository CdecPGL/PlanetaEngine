#pragma once

#include "CDrawGUI.h"

namespace planeta {
	class CDrawGraphGUI : public CDrawGUI {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDrawGraphGUI);
	public:
		using Super = CDrawGUI;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CDrawGraphGUI);
}