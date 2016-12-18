#include "CDrawGUI.h"
#include "ISceneInternal.h"
#include "DrawSystem.h"

namespace planeta {
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////

	class CDrawGUI::Impl_ {
	public:
		std::unique_ptr<private_::CDrawGUIManagerConnection> draw_system_connection;
	};

	//////////////////////////////////////////////////////////////////////////
	//CDrawGUI
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CDrawGUI) {
		registerer;
	}

	CDrawGUI::CDrawGUI():impl_(std::make_unique<Impl_>()) {}

	CDrawGUI::~CDrawGUI() = default;

	bool CDrawGUI::OnInitialized() {
		if (!Super::OnInitialized()) { return false; }
		impl_->draw_system_connection = scene_internal_interface().draw_system_internal_pointer()->RegisterCDrawGUI(shared_this<CDrawGUI>(),0);
		return impl_->draw_system_connection != nullptr;
	}

	void CDrawGUI::OnFinalized()noexcept {
		impl_->draw_system_connection->Remove();
		Super::OnFinalized();
	}

	bool CDrawGUI::OnActivated() {
		if (!Super::OnActivated()) { return false; }
		return impl_->draw_system_connection->Activte();
	}

	bool CDrawGUI::OnInactivated() {
		auto ret = impl_->draw_system_connection->Inactivate();
		return Super::OnInactivated() && ret;
	}

}