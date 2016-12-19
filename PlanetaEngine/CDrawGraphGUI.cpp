#include "CDrawGraphGUI.h"
#include "ResourceHolder.h"
#include "RGraph.h"
#include "ScreenDrawerGUI.h"
#include "CTransformGUI.h"
#include "RectAngle.h"

namespace planeta {
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////

	class CDrawGraphGUI::Impl_ {
	public:
		util::ResourceHolder<RGraph> graph_res;
		NonOwingPointer<CTransformGUI> my_c_trans_gui;
		RectAnglei graph_draw_area;
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
			.DeepCopyTarget(&CDrawGraphGUI::impl_);
	}

	CDrawGraphGUI::CDrawGraphGUI() :impl_(std::make_unique<Impl_>()) {}

	CDrawGraphGUI::~CDrawGraphGUI() = default;

	bool CDrawGraphGUI::resource_id(const std::string& res_id) {
		return impl_->graph_res.SetResourceByID(res_id);
	}

	void CDrawGraphGUI::DrawProc(ScreenDrawerGUI& drawer) {
		auto& trans = *impl_->my_c_trans_gui;
		drawer.DrawGraph(trans.position(), trans.size(), trans.pivot(), trans.rotation_rad(), impl_->graph_draw_area, impl_->reverse_flag, impl_->graph_res.resource_shared_pointer());
	}

	bool CDrawGraphGUI::GetOtherComponentsProc(const GOComponentGetter& com_getter) {
		if (!Super::GetOtherComponentsProc(com_getter)) { return false; }
		impl_->my_c_trans_gui = com_getter.GetComponent<CTransformGUI>();
		return true;
	}

	const planeta::RectAnglei& CDrawGraphGUI::draw_area() const {
		return impl_->graph_draw_area;
	}

	CDrawGraphGUI& CDrawGraphGUI::draw_area(const RectAnglei& rect) {
		impl_->graph_draw_area = rect;
		return *this;
	}

	const bool CDrawGraphGUI::reverse() const {
		return impl_->reverse_flag;
	}

	CDrawGraphGUI& CDrawGraphGUI::reverse(bool rev) {
		impl_->reverse_flag = rev;
		return *this;
	}

}