#include "planeta/core/ISceneInternal.hpp"
#include "planeta/core/DrawSystem.hpp"
#include "planeta/core/LogUtility.hpp"

#include "CDrawGUI.hpp"

namespace plnt {
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////

	class CDrawGUI::Impl_ {
	public:
		std::unique_ptr<private_::CDrawGUIManagerConnection> draw_system_connection;
		int priority;

		Impl_ &operator=(const Impl_ &obj) {
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
			.deep_copy_target(&CDrawGUI::impl_);
	}

	CDrawGUI::CDrawGUI(): impl_(std::make_unique<Impl_>()) { }

	CDrawGUI::~CDrawGUI() = default;

	void CDrawGUI::Draw(ScreenDrawerGUI &drawer) { DrawProc(drawer); }

	int CDrawGUI::draw_priority() const { return impl_->priority; }

	CDrawGUI &CDrawGUI::draw_priority(int priority) {
		impl_->priority = priority;
		impl_->draw_system_connection->ChangePriority(priority);
		return *this;
	}

	void CDrawGUI::OnInitialized() {
		super::OnInitialized();
		impl_->draw_system_connection = scene_internal_interface().draw_system_internal_pointer()->RegisterCDrawGUI(
			shared_this<CDrawGUI>(), impl_->priority);
		if (impl_->draw_system_connection == nullptr) { PE_LOG_ERROR("描画システムへの登録に失敗しました。"); }
	}

	void CDrawGUI::OnFinalized() noexcept {
		impl_->draw_system_connection->Remove();
		super::OnFinalized();
	}

	void CDrawGUI::OnActivated() {
		super::OnActivated();
		auto suceed = impl_->draw_system_connection->Activte();
		if (!suceed) { PE_LOG_ERROR("描画システムの有効化に失敗しました。"); }
	}

	void CDrawGUI::OnInactivated() {
		impl_->draw_system_connection->Inactivate();
		super::OnInactivated();
	}
}
