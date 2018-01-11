#include "CDrawGUI.hpp"
#include "planeta/ISceneInternal.hpp"
#include "planeta/DrawSystem.hpp"
#include "planeta/LogUtility.hpp"

namespace plnt {
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

	void CDrawGUI::OnInitialized() {
		Super::OnInitialized();
		impl_->draw_system_connection = scene_internal_interface().draw_system_internal_pointer()->RegisterCDrawGUI(shared_this<CDrawGUI>(), impl_->priority);
		if (impl_->draw_system_connection == nullptr) { PE_LOG_ERROR("•`‰æƒVƒXƒeƒ€‚Ö‚Ì“o˜^‚ÉŽ¸”s‚µ‚Ü‚µ‚½B"); }
	}

	void CDrawGUI::OnFinalized()noexcept {
		impl_->draw_system_connection->Remove();
		Super::OnFinalized();
	}

	void CDrawGUI::OnActivated() {
		Super::OnActivated();
		auto suceed = impl_->draw_system_connection->Activte();
		if (!suceed) {
			PE_LOG_ERROR("•`‰æƒVƒXƒeƒ€‚Ì—LŒø‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		}
	}

	void CDrawGUI::OnInactivated() {
		impl_->draw_system_connection->Inactivate();
		Super::OnInactivated();
	}

}