#include "planeta/core/i_scene_internal.hpp"
#include "..\core\draw_system.hpp"
#include "..\core\log_utility.hpp"

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
		impl_->draw_system_connection->change_priority(priority);
		return *this;
	}

	void CDrawGUI::on_initialized() {
		super::on_initialized();
		impl_->draw_system_connection = scene_internal_interface().draw_system_internal_pointer()->register_c_draw_gui(
			shared_this<CDrawGUI>(), impl_->priority);
		if (impl_->draw_system_connection == nullptr) { PE_LOG_ERROR("描画システムへの登録に失敗しました。"); }
	}

	void CDrawGUI::on_finalized() noexcept {
		impl_->draw_system_connection->remove();
		super::on_finalized();
	}

	void CDrawGUI::on_activated() {
		super::on_activated();
		auto suceed = impl_->draw_system_connection->active();
		if (!suceed) { PE_LOG_ERROR("描画システムの有効化に失敗しました。"); }
	}

	void CDrawGUI::on_inactivated() {
		impl_->draw_system_connection->inactivate();
		super::on_inactivated();
	}
}
