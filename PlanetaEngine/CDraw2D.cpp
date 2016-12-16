#include "CDraw2D.h"
#include "IGameObject.h"
#include "DrawSystem.h"
#include "CTransform2D.h"
#include "Matrix2_2.h"
#include "LogUtility.h"
#include "ISceneInternal.h"

namespace planeta {
	//////////////////////////////////////////////////////////////////////////
	//CDraw2D::Impl_
	//////////////////////////////////////////////////////////////////////////

	class CDraw2D::Impl_ {
	public:
		std::unique_ptr<private_::CDraw2DManagerConnection> draw_system_connection;
	};

	//////////////////////////////////////////////////////////////////////////
	//CDraw2D
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CDraw2D) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CDraw2D, draw_priority)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDraw2D, relative_position)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDraw2D, relative_rotation_rad)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDraw2D, relative_scale)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDraw2D, color)
			.ShallowCopyTarget(&CDraw2D::draw_priority_)
			.ShallowCopyTarget(&CDraw2D::position_)
			.ShallowCopyTarget(&CDraw2D::rotation_rad_)
			.ShallowCopyTarget(&CDraw2D::scale_)
			.ShallowCopyTarget(&CDraw2D::color_);
	}

	CDraw2D::CDraw2D():impl_(std::make_unique<Impl_>()), draw_priority_(0), rotation_rad_(0.0), scale_(1.0, 1.0) {

	}

	CDraw2D::~CDraw2D() = default;

	CDraw2D& CDraw2D::draw_priority(int priority) {
		draw_priority_ = priority;
		//ゲームオブジェクトがアクティブなら優先度更新
		UpdatePriority_();
		return *this;
	}

	Vector2Dd CDraw2D::GetDrawCenterPosition() const {
		Vector2Dd relation_position = math::RotationalTransformation(transform2d_->rotation_rad(), position_); //ゲームオブジェクトからの相対位置
		relation_position.x *= transform2d_->scale().x; //横方向拡大を反映
		relation_position.y *= transform2d_->scale().y; //縦方向拡大を反映
		return transform2d_->position() + relation_position;
	}

	double CDraw2D::GetDrawRotationRed() const {
		return transform2d_->rotation_rad() + rotation_rad_;
	}

	Vector2Dd CDraw2D::GetDrawScale() const {
		return Vector2Dd(transform2d_->scale().x * scale_.x, transform2d_->scale().y * scale_.y);
	}

	void CDraw2D::UpdatePriority_() {
		if (is_active()) {
			impl_->draw_system_connection->ChangePriority(draw_priority_);
		}
	}

	bool CDraw2D::OnInitialized() {
		impl_->draw_system_connection = scene_internal_interface().draw_system_internal_pointer()->RegisterCDraw2D(std::static_pointer_cast<CDraw2D>(shared_from_this()), draw_priority_);
		return impl_->draw_system_connection != nullptr;
	}

	void CDraw2D::OnFinalized()noexcept {
		impl_->draw_system_connection->Remove();
	}

	bool CDraw2D::OnActivated() {
		return impl_->draw_system_connection->Activte();
	}

	bool CDraw2D::OnInactivated() {
		return impl_->draw_system_connection->Inactivate();
	}

	bool CDraw2D::GetOtherComponentsProc(const GOComponentGetter& com_getter) {
		if (!Super::GetOtherComponentsProc(com_getter)) { return false; }
		transform2d_.reset(com_getter.GetComponent<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_ERROR("Transform2Dを取得できませんでした。");
			return false;
		}
		return true;
	}

}