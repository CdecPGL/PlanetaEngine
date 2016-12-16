#pragma once

#include "GameObjectSystemComponent.h"

namespace planeta {
	class CDrawGUI : public private_::GameObjectSystemComponent {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDrawGUI);
	public:
		using Super = private_::GameObjectSystemComponent;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CDrawGUI);
}
