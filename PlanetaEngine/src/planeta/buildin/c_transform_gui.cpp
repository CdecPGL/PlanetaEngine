#include <unordered_map>

#include "planeta/core/i_scene_internal.hpp"
#include "../core/transform_system.hpp"
#include "../core/log_utility.hpp"
#include "c_transform_gui.hpp"

namespace plnt {
	//////////////////////////////////////////////////////////////////////////
	//CTransformGUI::Impl_
	//////////////////////////////////////////////////////////////////////////

	class c_transform_gui::impl {
	public:
		rectangle_i rect;
		vector_2di position;
		vector_2di size{1, 1};
		double rotation_rad = 0;
		vector_2dd pivot{0.5, 0.5};

		impl &operator=(const impl &imp) {
			rect = imp.rect;
			position = imp.position;
			size = imp.size;
			rotation_rad = imp.rotation_rad;
			pivot = imp.pivot;
			return *this;
		}

		int t2d_id = -1;
	};

	//////////////////////////////////////////////////////////////////////////
	//CTransformGUI
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_transform_gui) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_transform_gui, position)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_transform_gui, size)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_transform_gui, rotation_rad)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_transform_gui, pivot)
			.deep_copy_target(&c_transform_gui::impl_);
	}

	c_transform_gui::c_transform_gui() : impl_(std::make_unique<impl>()) { }

	c_transform_gui::~c_transform_gui() = default;

	const vector_2di &c_transform_gui::position() const { return impl_->position; }

	c_transform_gui &c_transform_gui::position(const vector_2di &pos) {
		impl_->position = pos;
		return *this;
	}

	double c_transform_gui::rotation_rad() const { return impl_->rotation_rad; }

	c_transform_gui &c_transform_gui::rotation_rad(const double rota_rad) {
		impl_->rotation_rad = rota_rad;
		return *this;
	}

	c_transform_gui &c_transform_gui::size(const vector_2di &s) {
		impl_->size = s;
		return *this;
	}

	const vector_2di &c_transform_gui::size() const { return impl_->size; }

	c_transform_gui &c_transform_gui::pivot(const vector_2dd &p) {
		impl_->pivot = p;
		return *this;
	}

	const vector_2dd &c_transform_gui::pivot() const { return impl_->pivot; }

	void c_transform_gui::on_activated() {
		super::on_activated();
		//TransformSystemへ登録
		impl_->t2d_id = scene_internal_interface().transform_system_internal_pointer()->register_transform_gui(this);
		PE_VERIFY(impl_->t2d_id >= 0);
	}

	void c_transform_gui::on_inactivated() {
		PE_VERIFY(impl_->t2d_id >= 0);
		//TransformSystemから登録解除
		if (!scene_internal_interface().transform_system_internal_pointer()->remove_transform_gui(impl_->t2d_id)) {
			PE_LOG_FATAL("TransfromSystemからの登録解除に失敗しました。ID:", impl_->t2d_id);
		}
		super::on_inactivated();
	}

	/*const planeta::rectangle_i& CTransformGUI::rect() const {
		return impl_->rect;
	}*/
}
