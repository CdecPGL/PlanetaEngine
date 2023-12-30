#include "..\core\i_game_object.hpp"
#include "planeta/core/ScreenDrawerGUI.hpp"
#include "planeta/core/rectangle.hpp"
#include "..\core\resource_holder.hpp"

#include "CDrawGraphGUI.hpp"
#include "RGraph.hpp"
#include "CTransformGUI.hpp"

namespace plnt {
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////

	class CDrawGraphGUI::Impl_ {
	public:
		util::resource_holder<RGraph> graph_res;
		non_owing_pointer<CTransformGUI> my_c_trans_gui;
		rectangle_i graph_draw_area;
		bool reverse_flag;
	};

	//////////////////////////////////////////////////////////////////////////
	//CDrawGraphGUI
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CDrawGraphGUI) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CDrawGraphGUI, draw_area)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDrawGraphGUI, reverse)
			.PE_REFLECTABLE_CLASS_WRITEONLY_PROPERTY(CDrawGraphGUI, resource_id)
			.deep_copy_target(&CDrawGraphGUI::impl_);
	}

	CDrawGraphGUI::CDrawGraphGUI() : impl_(std::make_unique<Impl_>()) { }

	CDrawGraphGUI::~CDrawGraphGUI() = default;

	bool CDrawGraphGUI::resource_id(const std::string &res_id) { return impl_->graph_res.set_resource_by_id(res_id); }

	void CDrawGraphGUI::DrawProc(screen_drawer_gui &drawer) {
		auto &trans = *impl_->my_c_trans_gui;
		drawer.draw_graph(trans.position(), trans.size(), trans.pivot(), trans.rotation_rad(), impl_->graph_draw_area,
		                 impl_->reverse_flag, impl_->graph_res.resource_shared_pointer());
	}

	bool CDrawGraphGUI::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		impl_->my_c_trans_gui = com_getter.get_component<CTransformGUI>();
		return true;
	}

	const plnt::rectangle_i &CDrawGraphGUI::draw_area() const { return impl_->graph_draw_area; }

	CDrawGraphGUI &CDrawGraphGUI::draw_area(const rectangle_i &rect) {
		impl_->graph_draw_area = rect;
		return *this;
	}

	const bool CDrawGraphGUI::reverse() const { return impl_->reverse_flag; }

	CDrawGraphGUI &CDrawGraphGUI::reverse(bool rev) {
		impl_->reverse_flag = rev;
		return *this;
	}
}
