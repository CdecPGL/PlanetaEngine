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
		int priority;

		Impl_& operator=(const Impl_& obj) {
			priority = obj.priority;
			return *this;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//CDrawGUI
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CDrawGUI) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CDrawGUI, draw_priority)
			.DeepCopyTarget(&CDrawGUI::impl_);
	}

	CDrawGUI::CDrawGUI():impl_(std::make_unique<Impl_>()) {}

	CDrawGUI::~CDrawGUI() = default;

	void CDrawGUI::Draw(ScreenDrawerGUI& drawer) {
		DrawProc(drawer);
	}

	int CDrawGUI::draw_priority() const {
		return impl_->priority;
	}

	CDrawGUI& CDrawGUI::draw_priority(int priority) {
		impl_->priority = priority;
		impl_->draw_system_connection->ChangePriority(priority);
		return *this;
	}

	bool CDrawGUI::OnInitialized() {
		if (!Super::OnInitialized()) { return false; }
		impl_->draw_system_connection = scene_internal_interface().draw_system_internal_pointer()->RegisterCDrawGUI(shared_this<CDrawGUI>(), impl_->priority);
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

	void CDrawGUI::OnInactivated() {
		impl_->draw_system_connection->Inactivate();
		Super::OnInactivated();
	}

}