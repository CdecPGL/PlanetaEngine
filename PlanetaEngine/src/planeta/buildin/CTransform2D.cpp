#include <tuple>
#include <cassert>

#include "..\core\i_game_object.hpp"
#include "..\core\log_utility.hpp"
#include "..\core\transform_system.hpp"
#include "planeta/core/i_scene_internal.hpp"

#include "CGround2D.hpp"
#include "CTransform2D.hpp"
#include "CDumyGround2D.hpp"

namespace plnt {
	namespace {
		//ダミーグラウンド取得
		std::shared_ptr<CGround2D> GetDumyGround() {
			static auto dg = std::make_shared<CDumyGround2D>();
			return dg;
		}
	}

	//definitions
	namespace {
		struct TransformData {
			vector_2dd position;
			vector_2dd scale = vector_2dd(1.0, 1.0);
			double rotation_rad = 0.0;
		};

		struct PhisicalData {
			vector_2dd velocity;
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
		Impl_(): belonging_ground(GetDumyGround()) { }

	private:
		//各種データ
		std::tuple<TransformData, PhisicalData> global;
		std::tuple<TransformData, PhisicalData> ground;
		UpdateState last_update; //更新状況
		weak_pointer<CGround2D> belonging_ground;
		//地形更新イベントコネクション
		signal_connection ground_updated_event_connection;

		void PositionUpdated(CoordinationSpace space) {
			last_update.position = space;
			last_update.rotation = space; //位置に応じて回転度は変わる可能性があるので。
		}

		void ScaleUpdated(CoordinationSpace space) { last_update.scale = space; }

		void RotationUpdated(CoordinationSpace space) { last_update.rotation = space; }

		void VelocityUpdated(CoordinationSpace space) { last_update.velocity = space; }

		void RotaVelUpdated(CoordinationSpace space) { last_update.rota_vel = space; }

		void UpdateGlobalTransform() {
			auto &bground = *belonging_ground;
			//形状情報
			auto &glbl_t = std::get<TransformData>(global);
			auto &grnd_t = std::get<TransformData>(ground);
			if (last_update.position == CoordinationSpace::Ground) {
				glbl_t.position = bground.ConvertPositionGroundToGlobal(grnd_t.position);
				last_update.position = CoordinationSpace::None;
			}
			if (last_update.rotation == CoordinationSpace::Ground) {
				glbl_t.rotation_rad = bground.ConvertRotationGroundToGlobalWithGroundPosition(
					ground_position(), grnd_t.rotation_rad);
				last_update.rotation = CoordinationSpace::None;
			}
		}

		void UpdateGlobalPhisic() {
			auto &bground = *belonging_ground;
			//物理情報
			auto &glbl_p = std::get<PhisicalData>(global);
			auto &grnd_p = std::get<PhisicalData>(ground);
			if (last_update.velocity == CoordinationSpace::Ground) {
				glbl_p.velocity = bground.ConvertVelocityGroundToGlobalWithGroundPosition(
					ground_position(), grnd_p.velocity);
				last_update.velocity = CoordinationSpace::None;
			}
		}

		void UpdateGroundTransform() {
			auto &bground = *belonging_ground;
			//形状情報
			auto &glbl_t = std::get<TransformData>(global);
			auto &grnd_t = std::get<TransformData>(ground);
			if (last_update.position == CoordinationSpace::Global) {
				grnd_t.position = bground.ConvertPositionGlobalToGround(glbl_t.position);
				last_update.position = CoordinationSpace::None;
			}
			if (last_update.rotation == CoordinationSpace::Global) {
				grnd_t.rotation_rad = bground.ConvertRotationGlobalToGroundWithGroundPosition(
					grnd_t.position, glbl_t.rotation_rad);
				last_update.rotation = CoordinationSpace::None;
			}
		}

		void UpdateGroundPhisic() {
			auto &bground = *belonging_ground;
			//物理情報
			auto &glbl_p = std::get<PhisicalData>(global);
			auto &grnd_p = std::get<PhisicalData>(ground);
			if (last_update.velocity == CoordinationSpace::Global) {
				grnd_p.velocity = bground.ConvertVelocityGlobalToGroundWithGroundPosition(
					ground_position(), glbl_p.velocity);
				last_update.velocity = CoordinationSpace::None;
			}
		}

		//地形更新イベントハンドラ
		void OnGroudUpdated() {
			UpdateGroundTransform();
			UpdateGroundPhisic();
			last_update.position = CoordinationSpace::Ground;
			last_update.scale = CoordinationSpace::Ground;
			last_update.rotation = CoordinationSpace::Ground;
			last_update.velocity = CoordinationSpace::Ground;
			last_update.rota_vel = CoordinationSpace::Ground;
			UpdateGlobalTransform();
			UpdateGlobalPhisic();
		}

	public:
		//速度空間
		CoordinateSystem velocity_space = CoordinateSystem::Ground;
		//トランスフォーム2D_ID
		int t2d_id_ = -1;

		Impl_ &operator=(const Impl_ &obj) {
			global = obj.global;
			ground = obj.ground;
			belonging_ground = obj.belonging_ground;
			velocity_space = obj.velocity_space;
			return *this;
		}

		const vector_2dd &position() const {
			const_cast<Impl_ *>(this)->UpdateGlobalTransform();
			return std::get<TransformData>(global).position;
		}

		void position(const vector_2dd &pos) {
			std::get<TransformData>(global).position = pos;
			PositionUpdated(CoordinationSpace::Global);
		}

		const vector_2dd &scale() const {
			const_cast<Impl_ *>(this)->UpdateGlobalTransform();
			return std::get<TransformData>(global).scale;
		}

		void scale(const vector_2dd &s) {
			std::get<TransformData>(global).scale = s;
			ScaleUpdated(CoordinationSpace::Global);
		}

		const double rotation_rad() const {
			const_cast<Impl_ *>(this)->UpdateGlobalTransform();
			return std::get<TransformData>(global).rotation_rad;
		}

		void rotation_rad(double rota_rad) {
			std::get<TransformData>(global).rotation_rad = rota_rad;
			RotationUpdated(CoordinationSpace::Global);
		}

		const vector_2dd &ground_position() const {
			const_cast<Impl_ *>(this)->UpdateGroundTransform();
			return std::get<TransformData>(ground).position;
		}

		void ground_position(const vector_2dd &pos) {
			std::get<TransformData>(ground).position = pos;
			PositionUpdated(CoordinationSpace::Ground);
		}

		const double ground_rotation_rad() const {
			const_cast<Impl_ *>(this)->UpdateGroundTransform();
			return std::get<TransformData>(ground).rotation_rad;
		}

		void ground_rotation_rad(double rota_rad) {
			std::get<TransformData>(ground).rotation_rad = rota_rad;
			RotationUpdated(CoordinationSpace::Ground);
		}

		const vector_2dd &velocity() const {
			const_cast<Impl_ *>(this)->UpdateGlobalPhisic();
			return std::get<PhisicalData>(global).velocity;
		}

		void velocity(const vector_2dd &vel) {
			std::get<PhisicalData>(global).velocity = vel;
			VelocityUpdated(CoordinationSpace::Global);
		}

		const double rotation_velocity_rad() const {
			const_cast<Impl_ *>(this)->UpdateGlobalPhisic();
			return std::get<PhisicalData>(global).rota_vel_rad;
		}

		void rotation_velocity_rad(double rota_vel_rad) {
			std::get<PhisicalData>(global).rota_vel_rad = rota_vel_rad;
			RotaVelUpdated(CoordinationSpace::Global);
		}

		const vector_2dd &ground_velocity() const {
			const_cast<Impl_ *>(this)->UpdateGroundPhisic();
			return std::get<PhisicalData>(ground).velocity;
		}

		void ground_velocity(const vector_2dd &vel) {
			std::get<PhisicalData>(ground).velocity = vel;
			VelocityUpdated(CoordinationSpace::Ground);
		}

		CGround2D &cground() { return *belonging_ground; }

		weak_pointer<CGround2D> GetGround() const { return belonging_ground; }

		void SetGround(const weak_pointer<CGround2D> &g, bool keep_global_position) {
			if (belonging_ground) { ground_updated_event_connection.disconnect(); }

			if (keep_global_position) {
				UpdateGlobalTransform();
				UpdateGlobalPhisic();
			} else {
				UpdateGroundTransform();
				UpdateGroundPhisic();
			}

			belonging_ground = g;
			ground_updated_event_connection = belonging_ground->transform2d().updated.connect_function(
				std::bind(&Impl_::OnGroudUpdated, this));

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

		void GroundOffset(const vector_2dd &base_pos, const vector_2dd &offset) {
			ground_position(base_pos + cground().NormalizeGroundVectorWithGroundPosition(base_pos, offset));
		}

		void Initialize() {
			if (typeid(*belonging_ground) != typeid(CDumyGround2D)) {
				//所属地形があったら、自分を更新イベントリスナーに登録
				ground_updated_event_connection = belonging_ground->transform2d().updated.connect_function(
					std::bind(&Impl_::OnGroudUpdated, this));
			}
		}

		void Finalize() { ground_updated_event_connection.disconnect(); }
	};

	//////////////////////////////////////////////////////////////////////////
	//CTransform2D
	//////////////////////////////////////////////////////////////////////////
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CTransform2D) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CTransform2D, position)
			.PE_REFLECTABLE_CLASS_PROPERTY(CTransform2D, scale)
			.PE_REFLECTABLE_CLASS_PROPERTY(CTransform2D, rotation_rad)
			.PE_REFLECTABLE_CLASS_PROPERTY(CTransform2D, ground_position)
			.PE_REFLECTABLE_CLASS_PROPERTY(CTransform2D, ground_rotation_rad)
			.PE_REFLECTABLE_CLASS_PROPERTY(CTransform2D, velocity)
			.PE_REFLECTABLE_CLASS_PROPERTY(CTransform2D, rotation_velocity_rad)
			.PE_REFLECTABLE_CLASS_PROPERTY(CTransform2D, ground_velocity)
			.PE_REFLECTABLE_CLASS_PROPERTY(CTransform2D, velocity_space)
			.deep_copy_target(&CTransform2D::impl_);
	}

	CTransform2D::CTransform2D() : impl_(std::make_unique<Impl_>()) { };

	CTransform2D::~CTransform2D() = default;

	void CTransform2D::GroundMove(const vector_2dd &mov_pos) { GroundOffset(ground_position(), mov_pos); }

	void CTransform2D::GroundOffset(const vector_2dd &base_ground_pos, const vector_2dd &offset) {
		impl_->GroundOffset(base_ground_pos, offset);
	}

	void CTransform2D::Accelerate(const vector_2dd &acceleration) { velocity(velocity() + acceleration); }

	void CTransform2D::GroundAccelerate(const vector_2dd &ground_acceleration) {
		ground_velocity(ground_velocity() + ground_acceleration);
	}

	const vector_2dd &CTransform2D::position() const { return impl_->position(); }

	CTransform2D &CTransform2D::position(const vector_2dd &pos) {
		impl_->position(pos);
		return *this;
	}

	const vector_2dd &CTransform2D::scale() const { return impl_->scale(); }

	CTransform2D &CTransform2D::scale(const vector_2dd &s) {
		impl_->scale(s);
		return *this;
	}

	const double CTransform2D::rotation_rad() const { return impl_->rotation_rad(); }

	CTransform2D &CTransform2D::rotation_rad(double rota_rad) {
		impl_->rotation_rad(rota_rad);
		return *this;
	}

	const vector_2dd &CTransform2D::ground_position() const { return impl_->ground_position(); }

	CTransform2D &CTransform2D::ground_position(const vector_2dd &pos) {
		impl_->ground_position(pos);
		return *this;
	}

	const double CTransform2D::ground_rotation_rad() const { return impl_->ground_rotation_rad(); }

	CTransform2D &CTransform2D::ground_rotation_rad(double rota_rad) {
		impl_->ground_rotation_rad(rota_rad);
		return *this;
	}

	const vector_2dd &CTransform2D::velocity() const { return impl_->velocity(); }

	CTransform2D &CTransform2D::velocity(const vector_2dd &vel) {
		impl_->velocity(vel);
		return *this;
	}

	const double CTransform2D::rotation_velocity_rad() const { return impl_->rotation_velocity_rad(); }

	CTransform2D &CTransform2D::rotation_velocity_rad(double rota_vel_rad) {
		impl_->rotation_velocity_rad(rota_vel_rad);
		return *this;
	}

	const vector_2dd &CTransform2D::ground_velocity() const { return impl_->ground_velocity(); }

	CTransform2D &CTransform2D::ground_velocity(const vector_2dd &vel) {
		impl_->ground_velocity(vel);
		return *this;
	}

	const CGround2D &CTransform2D::ground() const { return const_cast<CTransform2D *>(this)->ground(); }

	CGround2D &CTransform2D::ground() { return impl_->cground(); }

	weak_pointer<CGround2D> CTransform2D::GetGround() const { return impl_->GetGround(); }

	bool CTransform2D::SetGround(const weak_pointer<i_game_object> &g, bool keep_global_position) {
		auto gcom = g->get_component<CGround2D>();
		if (gcom) {
			impl_->SetGround(gcom, keep_global_position);
			return true;
		} else {
			PE_LOG_ERROR("Groundコンポーネントを持たないGameObjectがTransformのGroundとして渡されました。");
			return false;
		}
	}

	void CTransform2D::velocity_space(CoordinateSystem space) { impl_->velocity_space = space; }

	CoordinateSystem CTransform2D::velocity_space() const { return impl_->velocity_space; }

	void CTransform2D::ApplyVelocity_() {
		rotation_rad(rotation_rad() + rotation_velocity_rad());
		switch (impl_->velocity_space) {
			case CoordinateSystem::Ground:
				GroundMove(ground_velocity());
				return;
			case CoordinateSystem::Global:
				position(position() + velocity());
				return;
			default:
				assert(false);
				return;
		}
	}

	void CTransform2D::on_initialized() {
		super::on_initialized();
		impl_->Initialize();
	}

	void CTransform2D::on_finalized() noexcept { impl_->Finalize(); }

	void CTransform2D::on_activated() {
		super::on_activated();
		//TransformSystemへ登録
		impl_->t2d_id_ = scene_internal_interface().transform_system_internal_pointer()->register_transform_2d(this);
		PE_VERIFY(impl_->t2d_id_ >= 0);
	}

	void CTransform2D::on_inactivated() {
		PE_VERIFY(impl_->t2d_id_ >= 0);
		//TransformSystemから登録解除
		if (!scene_internal_interface().transform_system_internal_pointer()->remove_transform_2d(impl_->t2d_id_)) {
			PE_LOG_FATAL("TransfromSystemからの登録解除に失敗しました。ID:", impl_->t2d_id_);
		}
		super::on_inactivated();
	}
}
