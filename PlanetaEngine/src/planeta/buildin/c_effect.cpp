#pragma warning(push)
#pragma warning(disable: 4100)
#include "EffekseerForDXLib.h"
#pragma warning(pop)

#include "planeta/core/game.hpp"
#include "planeta/core/i_resource_manager.hpp"
#include "../core/standard_resource_manager.hpp"
#include "../core/i_game_object.hpp"
#include "c_effect.hpp"
#include "c_transform_2d.hpp"
#include "REffect.hpp"
#include "TInstant.hpp"

namespace plnt {
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////

	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class c_effect::impl {
	public:
		impl &operator=(const impl &obj);

		bool create_effect_instance();
		// NOLINTNEXTLINE(modernize-use-nodiscard)
		bool start_effect() const;
		// NOLINTNEXTLINE(modernize-use-nodiscard)
		bool pause_effect() const;
		// NOLINTNEXTLINE(modernize-use-nodiscard)
		bool clear_effect_instance() const;

		bool set_resource_by_id(const std::string &resource_id);
		[[nodiscard]] bool get_effect_exits() const;
		void apply_transform_to_effect() const;

		void set_my_c_transform_2d(const non_owing_pointer<c_transform_2d> &com);
		void disconnect_my_c_transform_updated_event() const;
		void connect_my_c_transform_updated_event();

		bool loop_flag = false;
		bool auto_play = true;
		double expansion = 1.0;
		signal_connection trans_update_eve_connection;

	private:
		non_owing_pointer<c_transform_2d> my_c_transform_2d_ = {};
		std::shared_ptr<r_effect> r_effect_;
		int effect_handle_ = -1;
	};

	c_effect::impl &c_effect::impl::operator=(const impl &obj) {
		if (this == &obj) { return *this; }

		r_effect_ = obj.r_effect_;
		loop_flag = obj.loop_flag;
		auto_play = obj.auto_play;
		expansion = obj.expansion;
		return *this;
	}

	bool c_effect::impl::create_effect_instance() {
		clear_effect_instance();
		if (r_effect_) {
			const auto eff_mgr = GetEffekseer3DManager();
			const auto &trans = *my_c_transform_2d_;
			effect_handle_ = eff_mgr->Play(r_effect_->effekseer_effect(),
			                               static_cast<float>(trans.position().x),
			                               static_cast<float>(trans.position().y), 0);
			eff_mgr->SetPaused(effect_handle_, true);
			eff_mgr->SetShown(effect_handle_, false);
			return effect_handle_ >= 0;
		}
		return true;
	}

	bool c_effect::impl::start_effect() const {
		if (r_effect_) {
			const auto eff_mgr = GetEffekseer3DManager();
			eff_mgr->SetPaused(effect_handle_, false);
			eff_mgr->SetShown(effect_handle_, true);
		}
		return true;
	}

	bool c_effect::impl::pause_effect() const {
		if (r_effect_) {
			const auto eff_mgr = GetEffekseer3DManager();
			eff_mgr->SetPaused(effect_handle_, true);
			eff_mgr->SetShown(effect_handle_, false);
		}
		return true;
	}

	bool c_effect::impl::clear_effect_instance() const {
		if (r_effect_) {
			const auto eff_mgr = GetEffekseer3DManager();
			eff_mgr->StopEffect(effect_handle_);
		}
		return true;
	}

	bool c_effect::impl::set_resource_by_id(const std::string &resource_id) {
		if (const auto res = game::instance().resource_manager()->get_resource_by_id<r_effect>(resource_id)) {
			r_effect_ = res;
			return true;
		}
		PE_LOG_ERROR("エフェクトリソースの設定に失敗しました。指定IDは\"", resource_id, "\"。");
		return false;
	}

	bool c_effect::impl::get_effect_exits() const {
		if (effect_handle_ >= 0) {
			const auto eff_mgr = GetEffekseer3DManager();
			return eff_mgr->Exists(effect_handle_);
		} else { return false; }
	}

	void c_effect::impl::apply_transform_to_effect() const {
		const auto &trans = *my_c_transform_2d_;
		const auto eff_mgr = GetEffekseer3DManager();
		eff_mgr->SetLocation(effect_handle_,
		                     static_cast<float>(trans.position().x), static_cast<float>(trans.position().y), 0);
		eff_mgr->SetRotation(effect_handle_, 0, 0,
		                     static_cast<float>(trans.rotation_rad()));
		auto scl = trans.scale();
		scl *= expansion; //拡大率適用
		eff_mgr->SetScale(effect_handle_,
		                  static_cast<float>(scl.x), static_cast<float>(scl.y),
		                  static_cast<float>((scl.x + scl.y) / 2));
	}

	void c_effect::impl::connect_my_c_transform_updated_event() {
		if (my_c_transform_2d_) {
			trans_update_eve_connection.disconnect();
			trans_update_eve_connection = my_c_transform_2d_->updated.connect_function([this]() {
				apply_transform_to_effect();
			});
		}
	}

	void c_effect::impl::disconnect_my_c_transform_updated_event() const { trans_update_eve_connection.disconnect(); }

	void c_effect::impl::set_my_c_transform_2d(const non_owing_pointer<c_transform_2d> &com) {
		my_c_transform_2d_.reset(com);
		connect_my_c_transform_updated_event();
	}

	//////////////////////////////////////////////////////////////////////////
	//CEffect
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_effect) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_effect, loop_flag)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_effect, auto_play)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_effect, expansion)
			.PE_REFLECTABLE_CLASS_WRITEONLY_PROPERTY(c_effect, resource_id)
			.PE_REFLECTABLE_CLASS_READONLY_PROPERTY(c_effect, is_playing)
			.deep_copy_target(&c_effect::impl_);
	}

	c_effect::c_effect() : impl_(std::make_unique<impl>()) {}

	c_effect::~c_effect() = default;

	bool c_effect::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		impl_->set_my_c_transform_2d(com_getter.get_component<c_transform_2d>());
		return true;
	}

	void c_effect::on_initialized() {
		super::on_initialized();
		if (!impl_->get_effect_exits()) {
			if (!impl_->create_effect_instance()) {
				PE_LOG_ERROR("エフェクトインスタンスの作成に失敗しました。");
				return;
			}
		}
		const auto task = game_object().create_and_attach_task<t_instant>(task_slot::pre_draw_update_phase);
		task->set_execute_function([this] {
			impl_->apply_transform_to_effect();
			//エフェクトのループ確認
			if (impl_->loop_flag && !impl_->get_effect_exits()) {
				if (is_valid()) {
					if (!impl_->create_effect_instance()) { return; }
					if (is_active()) { impl_->start_effect(); }
				}
			}
		});
	}

	void c_effect::on_activated() {
		super::on_activated();
		impl_->connect_my_c_transform_updated_event();
		if (auto_play()) { if (!impl_->start_effect()) { PE_LOG_ERROR("エフェクトの開始に失敗しました。"); } }
	}

	void c_effect::on_inactivated() {
		impl_->disconnect_my_c_transform_updated_event();
		impl_->pause_effect();
		return super::on_inactivated();
	}

	void c_effect::on_finalized() noexcept {
		impl_->clear_effect_instance();
		super::on_finalized();
	}

	void c_effect::resource_id(const std::string &resource_id) const {
		if (!impl_->set_resource_by_id(resource_id)) { return; }
		if (is_valid()) {
			if (!impl_->create_effect_instance()) { return; }
			if (is_active() && auto_play()) { impl_->start_effect(); }
		}
	}

	bool c_effect::is_playing() const { return impl_->get_effect_exits() && is_active(); }

	// ReSharper disable once CppMemberFunctionMayBeConst
	void c_effect::loop_flag(const bool f) { impl_->loop_flag = f; }

	bool c_effect::loop_flag() const {
		const bool f = impl_->loop_flag;
		return f;
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void c_effect::auto_play(const bool f) { impl_->auto_play = f; }

	bool c_effect::auto_play() const { return impl_->auto_play; }

	// ReSharper disable once CppMemberFunctionMayBeConst
	void c_effect::expansion(const double e) { impl_->expansion = e; }

	double c_effect::expansion() const { return impl_->expansion; }

	bool c_effect::play() const { return impl_->start_effect(); }

	bool c_effect::stop() const { return impl_->pause_effect(); }
}
