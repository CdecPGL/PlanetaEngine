#include "CTransform2D.h"
#include <cassert>
#include "IGameObject.h"
#include "SystemLog.h"
#include "CGround2D.h"
#include "TransformSystem.h"
#include "SceneData.h"
#include <tuple>

namespace planeta_engine {
	//definitions
	namespace {
		struct TransformData {
			Vector2Dd position;
			Vector2Dd scale = Vector2Dd(1.0, 1.0);
			double rotation_rad = 0.0;
		};
		struct PhisicalData {
			Vector2Dd velocity;
			double rota_vel_rad = 0.0;
		};

		enum class CoordinationSpace { None, Global, Ground };
		struct UpdateState {
			CoordinationSpace position = CoordinationSpace::None;
			CoordinationSpace scale = CoordinationSpace::None;
			CoordinationSpace rotation = CoordinationSpace::None;
			CoordinationSpace velocity = CoordinationSpace::None;
			CoordinationSpace rota_vel = CoordinationSpace::None;
		};
	}
	//////////////////////////////////////////////////////////////////////////
	//Impl
	//////////////////////////////////////////////////////////////////////////
	class CTransform2D::Impl_ {
	public:
		Impl_::Impl_(){}
	private:
		//�o�b�t�@�f�[�^(�K�p�O�f�[�^)
		std::tuple<TransformData,PhisicalData> global;
		std::tuple<TransformData, PhisicalData> ground;
		UpdateState last_update; //�X�V��
		utility::WeakPointer<CGround2D> belonging_ground;
		//�n�`�X�V�C�x���g�R�l�N�V����
		utility::DelegateConnection ground_updated_event_connection;

		void PositionUpdated(CoordinationSpace space) {
			last_update.position = space;
		}
		void ScaleUpdated(CoordinationSpace space) {
			last_update.scale = space;
		}
		void RotationUpdated(CoordinationSpace space) {
			last_update.rotation = space;
		}
		void VelocityUpdated(CoordinationSpace space) {
			last_update.velocity = space;
		}
		void RotaVelUpdated(CoordinationSpace space) {
			last_update.rota_vel = space;
		}

		void UpdateGlobalTransform() {
			auto& bground = *belonging_ground;
			//�`����
			auto& glbl_t = std::get<TransformData>(global);
			auto& grnd_t = std::get<TransformData>(ground);
			if (last_update.position == CoordinationSpace::Ground) {
				glbl_t.position = bground.ConvertPositionGroundToGlobal(grnd_t.position);
				//�ʒu�ɉ����ĉ�]�x���ς��\��������̂ŁA��]�x�̍ŏI�X�V��Global�łȂ��������]�x���X�V����B
				if (last_update.rotation != CoordinationSpace::Global) {
					glbl_t.rotation_rad = bground.ConvertRotationGroundToGlobalWithGroundPosition(grnd_t.position, grnd_t.rotation_rad);
					last_update.rotation = CoordinationSpace::None; //�����ōX�V������Ăщ�]�x���X�V����K�v�͂Ȃ��̂�None�ɐݒ肵�Ă����B
				}
				last_update.position = CoordinationSpace::None;
			}
			if (last_update.rotation == CoordinationSpace::Ground) {
				glbl_t.rotation_rad = bground.ConvertRotationGroundToGlobalWithGroundPosition(grnd_t.position, grnd_t.rotation_rad);
				last_update.rotation = CoordinationSpace::None;
			}
		}
		void UpdateGlobalPhisic(){
			auto& bground = *belonging_ground;
			//�������
			auto& glbl_p = std::get<PhisicalData>(global);
			auto& grnd_p = std::get<PhisicalData>(ground);
			if (last_update.velocity == CoordinationSpace::Ground) {
				glbl_p.velocity = bground.ConvertVelocityGroundToGlobalWithGroundPosition(position(), grnd_p.velocity);
				last_update.velocity = CoordinationSpace::None;
			}
		}
		void UpdateGroundTransform() {
			auto& bground = *belonging_ground;
			//�`����
			auto& glbl_t = std::get<TransformData>(global);
			auto& grnd_t = std::get<TransformData>(ground);
			if (last_update.position == CoordinationSpace::Global) {
				grnd_t.position = bground.ConvertPositionGlobalToGround(glbl_t.position);
				last_update.position = CoordinationSpace::None;
			}
			if (last_update.rotation == CoordinationSpace::Global) {
				grnd_t.rotation_rad = bground.ConvertRotationGlobalToGroundWithGroundPosition(grnd_t.position, glbl_t.rotation_rad);
				last_update.rotation = CoordinationSpace::None;
			}
		}
		void UpdateGroundPhisic(){
			auto& bground = *belonging_ground;
			//�������
			auto& glbl_p = std::get<PhisicalData>(global);
			auto& grnd_p = std::get<PhisicalData>(ground);
			if (last_update.velocity == CoordinationSpace::Global) {
				grnd_p.velocity = bground.ConvertVelocityGlobalToGroundWithGroundPosition(position(), glbl_p.velocity);
				last_update.velocity = CoordinationSpace::None;
			}
		}

		//�n�`�X�V�C�x���g�n���h��
		void OnGroudUpdated() {

		}
	public:

		const Vector2Dd& position() const {
			const_cast<Impl_*>(this)->UpdateGlobalTransform();
			return std::get<TransformData>(global).position;
		}

		void  position(const Vector2Dd& pos) {
			std::get<TransformData>(global).position = pos;
			PositionUpdated(CoordinationSpace::Global);
			
		}

		const Vector2Dd& scale() const {
			const_cast<Impl_*>(this)->UpdateGlobalTransform();
			return std::get<TransformData>(global).scale;
		}

		void  scale(const Vector2Dd& s) {
			std::get<TransformData>(global).scale = s;
			ScaleUpdated(CoordinationSpace::Global);
			
		}

		const double rotation_rad() const {
			const_cast<Impl_*>(this)->UpdateGlobalTransform();
			return std::get<TransformData>(global).rotation_rad;
		}

		void rotation_rad(double rota_rad) {
			std::get<TransformData>(global).rotation_rad = rota_rad;
			RotationUpdated(CoordinationSpace::Global);
			
		}

		const Vector2Dd& ground_position() const {
			const_cast<Impl_*>(this)->UpdateGroundTransform();
			return std::get<TransformData>(ground).position;
		}

		void  ground_position(const Vector2Dd& pos) {
			std::get<TransformData>(ground).position = pos;
			PositionUpdated(CoordinationSpace::Ground);
			
		}

		const double ground_rotation_rad() const {
			const_cast<Impl_*>(this)->UpdateGroundTransform();
			return std::get<TransformData>(ground).rotation_rad;
		}

		void ground_rotation_rad(double rota_rad) {
			std::get<TransformData>(ground).rotation_rad = rota_rad;
			RotationUpdated(CoordinationSpace::Ground);
			
		}

		const Vector2Dd& velocity() const {
			const_cast<Impl_*>(this)->UpdateGlobalPhisic();
			return std::get<PhisicalData>(global).velocity;
		}

		void  velocity(const Vector2Dd& vel) {
			std::get<PhisicalData>(global).velocity = vel;
			VelocityUpdated(CoordinationSpace::Global);
			
		}

		const double rotation_velocity_rad() const {
			const_cast<Impl_*>(this)->UpdateGlobalPhisic();
			return std::get<PhisicalData>(global).rota_vel_rad;
		}

		void  rotation_velocity_rad(double rota_vel_rad) {
			std::get<PhisicalData>(global).rota_vel_rad = rota_vel_rad;
			RotaVelUpdated(CoordinationSpace::Global);
			
		}

		const Vector2Dd& ground_velocity()const {
			const_cast<Impl_*>(this)->UpdateGroundPhisic();
			return std::get<PhisicalData>(ground).velocity;
		}

		void  ground_velocity(const Vector2Dd& vel) {
			std::get<PhisicalData>(ground).velocity = vel;
			VelocityUpdated(CoordinationSpace::Ground);
			
		}

		CGround2D& cground() {
			return *belonging_ground;
		}

		utility::WeakPointer<CGround2D> GetGround() const {
			return belonging_ground;
		}

		void SetGround(const utility::WeakPointer<CGround2D>& g, bool keep_global_position) {
			if (keep_global_position) {
				UpdateGlobalTransform();
				UpdateGlobalPhisic();
			} else {
				UpdateGroundTransform();
				UpdateGroundPhisic();
			}

			belonging_ground = g;
			belonging_ground->transform2d().AddUpdatedEventHandler(utility::CreateDelegateHandlerAdder(this, &Impl_::OnGroudUpdated));

			if (keep_global_position) {
				last_update.position = CoordinationSpace::Global;
				last_update.rotation = CoordinationSpace::Global;
				last_update.velocity = CoordinationSpace::Global;
				UpdateGroundTransform();
				UpdateGroundPhisic();
			} else {
				last_update.position = CoordinationSpace::Ground;
				last_update.rotation = CoordinationSpace::Ground;
				last_update.velocity = CoordinationSpace::Ground;
				UpdateGlobalTransform();
				UpdateGlobalPhisic();
			}
		}

		void GroundOffset(const Vector2Dd& base_pos, const Vector2Dd& offset) {
			ground_position(base_pos + cground().NormalizeGroundVectorWithGroundPosition(base_pos, offset));
		}
		//���x���
		Space velocity_space = Space::Ground;
		//�g�����X�t�H�[��2D_ID
		int t2d_id_ = -1;
		//�X�V�C�x���g�f���Q�[�g
		utility::Delegate<void> updated_event_delegate;
	};

	//////////////////////////////////////////////////////////////////////////
	//CTransform2D
	//////////////////////////////////////////////////////////////////////////
	CTransform2D::CTransform2D() :impl_(std::make_unique<Impl_>()) {};

	CTransform2D::~CTransform2D() = default;

	void CTransform2D::GroundMove(const Vector2Dd& mov_pos) {
		GroundOffset(ground_position(), mov_pos);
	}

	void CTransform2D::GroundOffset(const Vector2Dd& base_ground_pos, const Vector2Dd& offset) {
		impl_->GroundOffset(base_ground_pos, offset);
	}

	void CTransform2D::Accelerate(const Vector2Dd& acceleration) {
		velocity(velocity() + acceleration);
	}

	void CTransform2D::GroundAccelerate(const Vector2Dd& ground_acceleration) {
		ground_velocity(ground_velocity() + ground_acceleration);
	}

	const Vector2Dd& CTransform2D::position() const {
		return impl_->position();
	}

	CTransform2D&  CTransform2D::position(const Vector2Dd& pos) {
		impl_->position(pos);
		return *this;
	}

	const Vector2Dd& CTransform2D::scale() const {
		return impl_->scale();
	}

	CTransform2D&  CTransform2D::scale(const Vector2Dd& s) {
		impl_->scale(s);
		return *this;
	}

	const double CTransform2D::rotation_rad() const {
		return impl_->rotation_rad();
	}

	CTransform2D& CTransform2D::rotation_rad(double rota_rad) {
		impl_->rotation_rad(rota_rad);
		return *this;
	}

	const Vector2Dd& CTransform2D::ground_position() const {
		return impl_->ground_position();
	}

	CTransform2D&  CTransform2D::ground_position(const Vector2Dd& pos) {
		impl_->ground_position(pos);
		return *this;
	}

	const double CTransform2D::ground_rotation_rad() const {
		return impl_->ground_rotation_rad();
	}

	CTransform2D& CTransform2D::ground_rotation_rad(double rota_rad) {
		impl_->ground_rotation_rad(rota_rad);
		return *this;
	}

	const Vector2Dd& CTransform2D::velocity() const {
		return impl_->velocity();
	}

	CTransform2D&  CTransform2D::velocity(const Vector2Dd& vel) {
		impl_->velocity(vel);
		return *this;
	}

	const double CTransform2D::rotation_velocity_rad() const {
		return impl_->rotation_velocity_rad();
	}

	CTransform2D&  CTransform2D::rotation_velocity_rad(double rota_vel_rad) {
		impl_->rotation_velocity_rad(rota_vel_rad);
		return *this;
	}

	const Vector2Dd& CTransform2D::ground_velocity()const {
		return impl_->ground_velocity();
	}

	CTransform2D&  CTransform2D::ground_velocity(const Vector2Dd& vel) {
		impl_->ground_velocity(vel);
		return *this;
	}

	const CGround2D& CTransform2D::ground() const {
		return const_cast<CTransform2D*>(this)->ground();
	}

	CGround2D& CTransform2D::ground() {
		return impl_->cground();
	}

	utility::WeakPointer<CGround2D> CTransform2D::GetGround() const {
		return impl_->GetGround();
	}

	bool CTransform2D::SetGround(const utility::WeakPointer<IGameObject>& g, bool keep_global_position) {
		auto gcom = g->GetComponent<CGround2D>();
		if (gcom) {
			impl_->SetGround(gcom, keep_global_position);
			return true;
		} else {
			PE_LOG_ERROR("Ground�R���|�[�l���g�������Ȃ�GameObject��Transform��Ground�Ƃ��ēn����܂����B");
			return false;
		}
	}

	void CTransform2D::velocity_space(Space space) {
		impl_->velocity_space = space;
	}

	void CTransform2D::ApplyVelocity_() {
		rotation_rad(rotation_rad() + rotation_velocity_rad());
		switch (impl_->velocity_space) {
		case Space::Ground:
			GroundMove(ground_velocity());
			return;
		case Space::Global:
			position(position() + velocity());
			return;
		default:
			assert(false);
			return;
		}
	}

	bool CTransform2D::OnInitialized() {
		return true;
	}

	void CTransform2D::OnFinalized()noexcept {

	}

	bool CTransform2D::OnActivated() {
		//TransformSystem�֓o�^
		impl_->t2d_id_ = scene_data_ref().transform_system.RegisterTransform2D(this);
		PE_VERIFY(impl_->t2d_id_ >= 0);
		return true;
	}

	bool CTransform2D::OnInactivated() {
		PE_VERIFY(impl_->t2d_id_ >= 0);
		//TransformSystem����o�^����
		if (scene_data_ref().transform_system.RemoveTransform2D(impl_->t2d_id_)) {
			return true;
		} else {
			PE_LOG_FATAL("TransfromSystem����̓o�^�����Ɏ��s���܂����BID:", impl_->t2d_id_);
			return false;
		}
	}

	utility::DelegateConnection CTransform2D::AddUpdatedEventHandler(utility::DelegateHandlerAdder<void>&& handler_adder) {
		return handler_adder(impl_->updated_event_delegate);
	}

}