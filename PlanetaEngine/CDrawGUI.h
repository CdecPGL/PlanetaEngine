#pragma once

#include "GameObjectSystemComponent.h"

namespace planeta {
	class CDrawGUI : public private_::GameObjectSystemComponent {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDrawGUI);
	public:
		using Super = private_::GameObjectSystemComponent;
		CDrawGUI();
		virtual ~CDrawGUI();
	protected:
		virtual bool OnInitialized()override;
		virtual void OnFinalized()noexcept override;
		virtual bool OnActivated()override;
		virtual void OnInactivated()override;
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CDrawGUI);
}
