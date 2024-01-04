#include "../core/i_game_object.hpp"
#include "../core/screen_drawer_gui.hpp"
#include "planeta/core/rectangle.hpp"
#include "../core/resource_holder.hpp"
#include "CDrawGraphGUI.hpp"
#include "RGraph.hpp"
#include "CTransformGUI.hpp"

namespace plnt {
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////

	class c_draw_graph_gui::impl {
	public:
		util::resource_holder<RGraph> graph_res;
		non_owing_pointer<c_transform_gui> my_c_trans_gui;
		rectangle_i graph_draw_area;
		bool reverse_flag;
	};

	//////////////////////////////////////////////////////////////////////////
	//CDrawGraphGUI
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_draw_graph_gui) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_graph_gui, draw_area)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_graph_gui, reverse)
			.PE_REFLECTABLE_CLASS_WRITEONLY_PROPERTY(c_draw_graph_gui, resource_id)
			.deep_copy_target(&c_draw_graph_gui::impl_);
	}

	c_draw_graph_gui::c_draw_graph_gui() : impl_(std::make_unique<impl>()) { }

	c_draw_graph_gui::~c_draw_graph_gui() = default;

	bool c_draw_graph_gui::resource_id(const std::string &res_id) const { return impl_->graph_res.set_resource_by_id(res_id); }

	void c_draw_graph_gui::draw_proc(screen_drawer_gui &drawer) {
		const auto &trans = *impl_->my_c_trans_gui;
		drawer.draw_graph(trans.position(), trans.size(), trans.pivot(), trans.rotation_rad(), impl_->graph_draw_area,
		                 impl_->reverse_flag, impl_->graph_res.resource_shared_pointer());
	}

	bool c_draw_graph_gui::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		impl_->my_c_trans_gui = com_getter.get_component<c_transform_gui>();
		return true;
	}

	const plnt::rectangle_i &c_draw_graph_gui::draw_area() const { return impl_->graph_draw_area; }

	c_draw_graph_gui &c_draw_graph_gui::draw_area(const rectangle_i &rect) {
		impl_->graph_draw_area = rect;
		return *this;
	}

	bool c_draw_graph_gui::reverse() const { return impl_->reverse_flag; }

	c_draw_graph_gui &c_draw_graph_gui::reverse(const bool rev) {
		impl_->reverse_flag = rev;
		return *this;
	}
}
