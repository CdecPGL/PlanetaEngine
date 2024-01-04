#include "planeta/core/i_scene_internal.hpp"
#include "../core/draw_system.hpp"
#include "../core/log_utility.hpp"
#include "CDrawGUI.hpp"

namespace plnt {
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////

	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class c_draw_gui::impl {
	public:
		std::unique_ptr<private_::c_draw_gui_manager_connection> draw_system_connection;
		int priority;

		impl &operator=(const impl &obj) {
			priority = obj.priority;
			return *this;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//CDrawGUI
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_draw_gui) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_gui, draw_priority)
			.deep_copy_target(&c_draw_gui::impl_);
	}

	c_draw_gui::c_draw_gui(): impl_(std::make_unique<impl>()) {}

	c_draw_gui::~c_draw_gui() = default;

	void c_draw_gui::draw(screen_drawer_gui &drawer) { draw_proc(drawer); }

	int c_draw_gui::draw_priority() const { return impl_->priority; }

	c_draw_gui &c_draw_gui::draw_priority(const int priority) {
		impl_->priority = priority;
		impl_->draw_system_connection->change_priority(priority);
		return *this;
	}

	void c_draw_gui::on_initialized() {
		super::on_initialized();
		impl_->draw_system_connection = scene_internal_interface().draw_system_internal_pointer()->register_c_draw_gui(
			shared_this<c_draw_gui>(), impl_->priority);
		if (impl_->draw_system_connection == nullptr) { PE_LOG_ERROR("描画システムへの登録に失敗しました。"); }
	}

	void c_draw_gui::on_finalized() noexcept {
		impl_->draw_system_connection->remove();
		super::on_finalized();
	}

	void c_draw_gui::on_activated() {
		super::on_activated();
		if (const auto succeeded = impl_->draw_system_connection->active(); !succeeded) {
			PE_LOG_ERROR("描画システムの有効化に失敗しました。");
		}
	}

	void c_draw_gui::on_inactivated() {
		impl_->draw_system_connection->inactivate();
		super::on_inactivated();
	}
}
