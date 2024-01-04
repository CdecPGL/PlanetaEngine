#include <tuple>
#include <cassert>

#include "../core/i_game_object.hpp"
#include "../core/log_utility.hpp"
#include "../core/transform_system.hpp"
#include "planeta/core/i_scene_internal.hpp"
#include "c_ground_2d.hpp"
#include "c_transform_2d.hpp"
#include "c_dummy_ground_2d.hpp"

namespace plnt {
	namespace {
		//ダミーグラウンド取得
		std::shared_ptr<c_ground_2d> get_dummy_ground() {
			static auto dg = std::make_shared<c_dummy_ground_2d>();
			return dg;
		}
	}

	//definitions
	namespace {
		struct transform_data {
			vector_2dd position;
			vector_2dd scale = vector_2dd(1.0, 1.0);
			double rotation_rad = 0.0;
		};

		struct physical_data {
			vector_2dd velocity;
			double rota_vel_rad = 0.0;
		};

		enum class coordination_space { none, global, ground };

		struct update_state {
			coordination_space position = coordination_space::none;
			coordination_space scale = coordination_space::none;
			coordination_space rotation = coordination_space::none;
			coordination_space velocity = coordination_space::none;
			coordination_space rota_vel = coordination_space::none;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	//Impl
	//////////////////////////////////////////////////////////////////////////
	class c_transform_2d::impl {
	public:
		impl(): belonging_ground_(get_dummy_ground()) {}

	private:
		//各種データ
		std::tuple<transform_data, physical_data> global_;
		std::tuple<transform_data, physical_data> ground_;
		update_state last_update_; //更新状況
		weak_pointer<c_ground_2d> belonging_ground_;
		//地形更新イベントコネクション
		signal_connection ground_updated_event_connection_;

		void position_updated(const coordination_space space) {
			last_update_.position = space;
			last_update_.rotation = space; //位置に応じて回転度は変わる可能性があるので。
		}

		void scale_updated(const coordination_space space) { last_update_.scale = space; }

		void rotation_updated(const coordination_space space) { last_update_.rotation = space; }

		void velocity_updated(const coordination_space space) { last_update_.velocity = space; }

		void rota_vel_updated(const coordination_space space) { last_update_.rota_vel = space; }

		void update_global_transform() {
			const auto &bg_round = *belonging_ground_;
			//形状情報
			// ReSharper disable once CppUseStructuredBinding
			auto &global_t = std::get<transform_data>(global_);
			// ReSharper disable once CppUseStructuredBinding
			const auto &ground_t = std::get<transform_data>(ground_);
			if (last_update_.position == coordination_space::ground) {
				global_t.position = bg_round.convert_position_ground_to_global(ground_t.position);
				last_update_.position = coordination_space::none;
			}
			if (last_update_.rotation == coordination_space::ground) {
				global_t.rotation_rad = bg_round.convert_rotation_ground_to_global_with_ground_position(
					ground_position(), ground_t.rotation_rad);
				last_update_.rotation = coordination_space::none;
			}
		}

		void update_global_physics() {
			const auto &bg_round = *belonging_ground_;
			//物理情報
			// ReSharper disable once CppUseStructuredBinding
			auto &global_p = std::get<physical_data>(global_);
			// ReSharper disable once CppUseStructuredBinding
			const auto &ground_p = std::get<physical_data>(ground_);
			if (last_update_.velocity == coordination_space::ground) {
				global_p.velocity = bg_round.convert_velocity_ground_to_global_with_ground_position(
					ground_position(), ground_p.velocity);
				last_update_.velocity = coordination_space::none;
			}
		}

		void update_ground_transform() {
			const auto &bg_round = *belonging_ground_;
			//形状情報
			//  ReSharper disable once CppUseStructuredBinding
			const auto &global_t = std::get<transform_data>(global_);
			// ReSharper disable once CppUseStructuredBinding
			auto &ground_t = std::get<transform_data>(ground_);
			if (last_update_.position == coordination_space::global) {
				ground_t.position = bg_round.convert_position_global_to_ground(global_t.position);
				last_update_.position = coordination_space::none;
			}
			if (last_update_.rotation == coordination_space::global) {
				ground_t.rotation_rad = bg_round.convert_rotation_global_to_ground_with_ground_position(
					ground_t.position, global_t.rotation_rad);
				last_update_.rotation = coordination_space::none;
			}
		}

		void update_ground_physics() {
			const auto &bg_round = *belonging_ground_;
			//物理情報
			//  ReSharper disable once CppUseStructuredBinding
			const auto &global_p = std::get<physical_data>(global_);
			//  ReSharper disable once CppUseStructuredBinding
			auto &ground_p = std::get<physical_data>(ground_);
			if (last_update_.velocity == coordination_space::global) {
				ground_p.velocity = bg_round.convert_velocity_global_to_ground_with_ground_position(
					ground_position(), global_p.velocity);
				last_update_.velocity = coordination_space::none;
			}
		}

		//地形更新イベントハンドラ
		void on_ground_updated() {
			update_ground_transform();
			update_ground_physics();
			last_update_.position = coordination_space::ground;
			last_update_.scale = coordination_space::ground;
			last_update_.rotation = coordination_space::ground;
			last_update_.velocity = coordination_space::ground;
			last_update_.rota_vel = coordination_space::ground;
			update_global_transform();
			update_global_physics();
		}

	public:
		//速度空間
		coordinate_system velocity_space = coordinate_system::ground;
		//トランスフォーム2D_ID
		int t2d_id = -1;

		impl &operator=(const impl &obj) {
			global_ = obj.global_;
			ground_ = obj.ground_;
			belonging_ground_ = obj.belonging_ground_;
			velocity_space = obj.velocity_space;
			return *this;
		}

		[[nodiscard]] const vector_2dd &position() const {
			const_cast<impl *>(this)->update_global_transform();
			return std::get<transform_data>(global_).position;
		}

		void position(const vector_2dd &pos) {
			std::get<transform_data>(global_).position = pos;
			position_updated(coordination_space::global);
		}

		[[nodiscard]] const vector_2dd &scale() const {
			const_cast<impl *>(this)->update_global_transform();
			return std::get<transform_data>(global_).scale;
		}

		void scale(const vector_2dd &s) {
			std::get<transform_data>(global_).scale = s;
			scale_updated(coordination_space::global);
		}

		[[nodiscard]] double rotation_rad() const {
			const_cast<impl *>(this)->update_global_transform();
			return std::get<transform_data>(global_).rotation_rad;
		}

		void rotation_rad(const double rota_rad) {
			std::get<transform_data>(global_).rotation_rad = rota_rad;
			rotation_updated(coordination_space::global);
		}

		[[nodiscard]] const vector_2dd &ground_position() const {
			const_cast<impl *>(this)->update_ground_transform();
			return std::get<transform_data>(ground_).position;
		}

		void ground_position(const vector_2dd &pos) {
			std::get<transform_data>(ground_).position = pos;
			position_updated(coordination_space::ground);
		}

		[[nodiscard]] double ground_rotation_rad() const {
			const_cast<impl *>(this)->update_ground_transform();
			return std::get<transform_data>(ground_).rotation_rad;
		}

		void ground_rotation_rad(const double rota_rad) {
			std::get<transform_data>(ground_).rotation_rad = rota_rad;
			rotation_updated(coordination_space::ground);
		}

		[[nodiscard]] const vector_2dd &velocity() const {
			const_cast<impl *>(this)->update_global_physics();
			return std::get<physical_data>(global_).velocity;
		}

		void velocity(const vector_2dd &vel) {
			std::get<physical_data>(global_).velocity = vel;
			velocity_updated(coordination_space::global);
		}

		[[nodiscard]] double rotation_velocity_rad() const {
			const_cast<impl *>(this)->update_global_physics();
			return std::get<physical_data>(global_).rota_vel_rad;
		}

		void rotation_velocity_rad(const double rota_vel_rad) {
			std::get<physical_data>(global_).rota_vel_rad = rota_vel_rad;
			rota_vel_updated(coordination_space::global);
		}

		[[nodiscard]] const vector_2dd &ground_velocity() const {
			const_cast<impl *>(this)->update_ground_physics();
			return std::get<physical_data>(ground_).velocity;
		}

		void ground_velocity(const vector_2dd &vel) {
			std::get<physical_data>(ground_).velocity = vel;
			velocity_updated(coordination_space::ground);
		}

		c_ground_2d &c_ground() const { return *belonging_ground_; }

		[[nodiscard]] weak_pointer<c_ground_2d> get_ground() const { return belonging_ground_; }

		void set_ground(const weak_pointer<c_ground_2d> &g, const bool keep_global_position) {
			if (belonging_ground_) { ground_updated_event_connection_.disconnect(); }

			if (keep_global_position) {
				update_global_transform();
				update_global_physics();
			} else {
				update_ground_transform();
				update_ground_physics();
			}

			belonging_ground_ = g;
			ground_updated_event_connection_ = belonging_ground_->transform2d().updated.connect_function(
				[this] { on_ground_updated(); });

			if (keep_global_position) {
				last_update_.position = coordination_space::global;
				last_update_.rotation = coordination_space::global;
				last_update_.velocity = coordination_space::global;
				update_ground_transform();
				update_ground_physics();
			} else {
				last_update_.position = coordination_space::ground;
				last_update_.rotation = coordination_space::ground;
				last_update_.velocity = coordination_space::ground;
				update_global_transform();
				update_global_physics();
			}
		}

		void ground_offset(const vector_2dd &base_pos, const vector_2dd &offset) {
			ground_position(base_pos + c_ground().normalize_ground_vector_with_ground_position(base_pos, offset));
		}

		void initialize() {
			// NOLINTNEXTLINE(clang-diagnostic-potentially-evaluated-expression)
			if (typeid(*belonging_ground_) != typeid(c_dummy_ground_2d)) {
				//所属地形があったら、自分を更新イベントリスナーに登録
				ground_updated_event_connection_ = belonging_ground_->transform2d().updated.connect_function(
					[this] { on_ground_updated(); });
			}
		}

		void finalize() const { ground_updated_event_connection_.disconnect(); }
	};

	//////////////////////////////////////////////////////////////////////////
	//CTransform2D
	//////////////////////////////////////////////////////////////////////////
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_transform_2d) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_transform_2d, position)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_transform_2d, scale)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_transform_2d, rotation_rad)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_transform_2d, ground_position)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_transform_2d, ground_rotation_rad)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_transform_2d, velocity)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_transform_2d, rotation_velocity_rad)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_transform_2d, ground_velocity)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_transform_2d, velocity_space)
			.deep_copy_target(&c_transform_2d::impl_);
	}

	c_transform_2d::c_transform_2d() : impl_(std::make_unique<impl>()) {};

	c_transform_2d::~c_transform_2d() = default;

	void c_transform_2d::ground_move(const vector_2dd &mov_pos) const { ground_offset(ground_position(), mov_pos); }

	void c_transform_2d::ground_offset(const vector_2dd &base_ground_pos, const vector_2dd &offset) const {
		impl_->ground_offset(base_ground_pos, offset);
	}

	void c_transform_2d::accelerate(const vector_2dd &acceleration) { velocity(velocity() + acceleration); }

	void c_transform_2d::ground_accelerate(const vector_2dd &ground_acceleration) {
		ground_velocity(ground_velocity() + ground_acceleration);
	}

	const vector_2dd &c_transform_2d::position() const { return impl_->position(); }

	c_transform_2d &c_transform_2d::position(const vector_2dd &pos) {
		impl_->position(pos);
		return *this;
	}

	const vector_2dd &c_transform_2d::scale() const { return impl_->scale(); }

	c_transform_2d &c_transform_2d::scale(const vector_2dd &s) {
		impl_->scale(s);
		return *this;
	}

	double c_transform_2d::rotation_rad() const { return impl_->rotation_rad(); }

	c_transform_2d &c_transform_2d::rotation_rad(const double rota_rad) {
		impl_->rotation_rad(rota_rad);
		return *this;
	}

	const vector_2dd &c_transform_2d::ground_position() const { return impl_->ground_position(); }

	c_transform_2d &c_transform_2d::ground_position(const vector_2dd &pos) {
		impl_->ground_position(pos);
		return *this;
	}

	double c_transform_2d::ground_rotation_rad() const { return impl_->ground_rotation_rad(); }

	c_transform_2d &c_transform_2d::ground_rotation_rad(const double rota_rad) {
		impl_->ground_rotation_rad(rota_rad);
		return *this;
	}

	const vector_2dd &c_transform_2d::velocity() const { return impl_->velocity(); }

	c_transform_2d &c_transform_2d::velocity(const vector_2dd &vel) {
		impl_->velocity(vel);
		return *this;
	}

	double c_transform_2d::rotation_velocity_rad() const { return impl_->rotation_velocity_rad(); }

	c_transform_2d &c_transform_2d::rotation_velocity_rad(const double rota_vel_rad) {
		impl_->rotation_velocity_rad(rota_vel_rad);
		return *this;
	}

	const vector_2dd &c_transform_2d::ground_velocity() const { return impl_->ground_velocity(); }

	c_transform_2d &c_transform_2d::ground_velocity(const vector_2dd &vel) {
		impl_->ground_velocity(vel);
		return *this;
	}

	const c_ground_2d &c_transform_2d::ground() const { return const_cast<c_transform_2d *>(this)->ground(); }

	c_ground_2d &c_transform_2d::ground() { return impl_->c_ground(); }

	weak_pointer<c_ground_2d> c_transform_2d::get_ground() const { return impl_->get_ground(); }

	// ReSharper disable once CppMemberFunctionMayBeConst
	bool c_transform_2d::set_ground(const weak_pointer<i_game_object> &g, const bool keep_global_position) {
		if (const auto g_com = g->get_component<c_ground_2d>()) {
			impl_->set_ground(g_com, keep_global_position);
			return true;
		}
		PE_LOG_ERROR("Groundコンポーネントを持たないGameObjectがTransformのGroundとして渡されました。");
		return false;
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void c_transform_2d::velocity_space(const coordinate_system space) { impl_->velocity_space = space; }

	coordinate_system c_transform_2d::velocity_space() const { return impl_->velocity_space; }

	void c_transform_2d::apply_velocity() {
		rotation_rad(rotation_rad() + rotation_velocity_rad());
		switch (impl_->velocity_space) {
			case coordinate_system::ground:
				ground_move(ground_velocity());
				return;
			case coordinate_system::global:
				position(position() + velocity());
				return;
		}

		assert(false);
	}

	void c_transform_2d::on_initialized() {
		super::on_initialized();
		impl_->initialize();
	}

	void c_transform_2d::on_finalized() noexcept { impl_->finalize(); }

	void c_transform_2d::on_activated() {
		super::on_activated();
		//TransformSystemへ登録
		impl_->t2d_id = scene_internal_interface().transform_system_internal_pointer()->register_transform_2d(this);
		PE_VERIFY(impl_->t2d_id >= 0);
	}

	void c_transform_2d::on_inactivated() {
		PE_VERIFY(impl_->t2d_id >= 0);
		//TransformSystemから登録解除
		if (!scene_internal_interface().transform_system_internal_pointer()->remove_transform_2d(impl_->t2d_id)) {
			PE_LOG_FATAL("TransfromSystemからの登録解除に失敗しました。ID:", impl_->t2d_id);
		}
		super::on_inactivated();
	}
}
