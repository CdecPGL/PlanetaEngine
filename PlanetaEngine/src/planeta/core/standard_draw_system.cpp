#include <functional>

#include "DxLib.h"

#pragma warning(push)
#pragma warning(disable: 4100)
#include "EffekseerForDXLib.h"
#pragma warning(pop)

#include "game.hpp"
#include "rendering_manager.hpp"
#include "config_manager.hpp"
#include "standard_draw_system.hpp"
#include "planeta/buildin/CDraw2D.hpp"
#include "planeta/buildin/CDrawGUI.hpp"
#include "log_utility.hpp"
#include "screen_drawer_2d.hpp"
#include "screen_drawer_gui.hpp"
#include "planeta/buildin/CCamera2D.hpp"
#include "i_debug_manager.hpp"

namespace plnt::private_ {
	//////////////////////////////////////////////////////////////////////////
	//ManagerConnections
	//////////////////////////////////////////////////////////////////////////
	//CDraw用
	template <class ComType, class ManagerConnectionType>
	class standard_manager_connection final : public ManagerConnectionType {
	public:
		standard_manager_connection(standard_draw_system::com_holder<ComType> &com_holder,
		                            typename standard_draw_system::com_map_type<ComType>::iterator com_map_itr)
			: com_holder_(com_holder), com_map_itr_(std::move(com_map_itr)) {}

		bool active() override { return com_holder_.activate(com_map_itr_); }

		bool inactivate() override { return com_holder_.inactivate(com_map_itr_); }

		bool remove() override { return com_holder_.remove(com_map_itr_); }

		bool change_priority(int priority) override { return com_holder_.change_priority(com_map_itr_, priority); }

	private:
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
		standard_draw_system::com_holder<ComType> &com_holder_;
		typename standard_draw_system::com_map_type<ComType>::iterator com_map_itr_;
	};

	using standard_c_draw_2d_manager_connection = standard_manager_connection<c_draw_2d, c_draw_2d_manager_connection>;
	using standard_c_draw_gui_manager_connection = standard_manager_connection<c_draw_gui, c_draw_gui_manager_connection>;

	//CCamera2D用
	class standard_c_camera_2d_manager_connection final : public c_camera_2d_manager_connection {
	public:
		explicit standard_c_camera_2d_manager_connection(std::function<bool()> &&remove_handler)
			: remove_handler_(std::move(remove_handler)) {}

		bool remove() override { return remove_handler_(); }

	private:
		std::function<bool()> remove_handler_;
	};

	//////////////////////////////////////////////////////////////////////////
	//StandardDrawSystem
	//////////////////////////////////////////////////////////////////////////

	standard_draw_system::standard_draw_system() = default;

	standard_draw_system::~standard_draw_system() = default;

	void standard_draw_system::execute_draw() {
		//2DDraw
		c_draw_2d_holder_.iterate([&sd_2d = *screen_drawer_2d_](c_draw_2d &com) { com.draw(sd_2d); });
	}

	void standard_draw_system::execute_draw_gui() {
		//GUIDraw
		c_draw_gui_holder_.iterate([&sd_gui = *screen_drawer_gui_](c_draw_gui &com) { com.draw(sd_gui); });
	}

	void standard_draw_system::update() {}

	bool standard_draw_system::initialize() {
		screen_ = game::instance().rendering_manager()->get_main_screen();
		if (!screen_) { return false; }
		screen_drawer_2d_ = std::make_unique<screen_drawer_2d>(*screen_);
		screen_drawer_gui_ = std::make_unique<screen_drawer_gui>(*screen_);
		return true;
	}

	void standard_draw_system::finalize() { return; }

	void standard_draw_system::debug_information_add_handle(i_debug_information_adder &di_adder) {
		di_adder.add_line("-----GameObjectDrawSystem-----");
		di_adder.add_line_v("2D描画コンポーネント(有効数/総数):", c_draw_2d_holder_.active_count(), "/",
		                    c_draw_2d_holder_.all_count());
		di_adder.add_line_v("GUI描画コンポーネント(有効数/総数):", c_draw_gui_holder_.active_count(), "/",
		                    c_draw_gui_holder_.all_count());
	}

	void standard_draw_system::apply_camera_state() {
		if (camera2d_) {
			const double scale = camera2d_->expansion();
			const double rota_rad = camera2d_->rotation_rad();
			const vector_2dd pos = camera2d_->position();
			SetupCamera_Ortho(static_cast<float>(game::instance().config_manager()->draw_size().y / scale));
			SetCameraPositionAndAngle(VGet(static_cast<float>(pos.x), static_cast<float>(pos.y), GetCameraPosition().z),
			                          GetCameraAngleVRotate(), GetCameraAngleHRotate(), static_cast<float>(rota_rad));
			//Effekseerのカメラを同期
			Effekseer_Sync3DSetting();
		} else { PE_LOG_WARNING("シーン内にカメラ2Dが登録されていません。"); }
	}

	vector_2dd standard_draw_system::covert_position_screen_space_to_game_object_space(
		const vector_2di &gui_space_pos) const {
		const auto [x, y, z] = ConvScreenPosToWorldPos(VGet(static_cast<float>(gui_space_pos.x),
		                                                    static_cast<float>(gui_space_pos.y),
		                                                    0.0f));
		return {x, y};
	}

	vector_2di standard_draw_system::covert_position_game_object_space_to_screen_space(
		const vector_2dd &game_object_space_pos) const {
		const auto [x, y, z] = ConvWorldPosToScreenPos(VGet(static_cast<float>(game_object_space_pos.x),
		                                                    static_cast<float>(game_object_space_pos.y), 0.0f));
		return {static_cast<int>(x), static_cast<int>(y)};
	}

	std::unique_ptr<c_draw_2d_manager_connection> standard_draw_system::register_c_draw_2d(
		const std::shared_ptr<c_draw_2d> &draw_component, const int priority) {
		//コンポーネントを登録
		auto [is_succeeded, it] = c_draw_2d_holder_.add(draw_component, priority);
		if (!is_succeeded) { return nullptr; }
		//登録できていたらマネージャConnectionを作成し返す
		return std::make_unique<standard_c_draw_2d_manager_connection>(c_draw_2d_holder_, it);
	}

	std::unique_ptr<c_draw_gui_manager_connection> standard_draw_system::register_c_draw_gui(
		const std::shared_ptr<c_draw_gui> &draw_component, const int priority) {
		//コンポーネントを登録
		auto [is_succeeded, it] = c_draw_gui_holder_.add(draw_component, priority);
		if (!is_succeeded) { return nullptr; }
		//登録できていたらマネージャConnectionを作成し返す
		return std::make_unique<standard_c_draw_gui_manager_connection>(c_draw_gui_holder_, it);
	}

	std::unique_ptr<c_camera_2d_manager_connection> standard_draw_system::register_c_camera_2d(
		const std::shared_ptr<c_camera_2d> &camera_component) {
		if (camera2d_) {
			PE_LOG_ERROR("シーン内の複数カメラはサポートされていません。初めに登録されたカメラのみが有効です。カメラコンポーネントを持つゲームオブジェクトが複数存在する可能性があります。");
			return nullptr;
		}
		camera2d_ = camera_component;
		//Connectionを作成して返す
		auto mgr_cnc = std::make_unique<standard_c_camera_2d_manager_connection>(
			[this,cc_ptr = camera_component.get()] {
				if (camera2d_ != nullptr && camera2d_.get() == cc_ptr) {
					camera2d_.reset();
					return true;
				}
				PE_LOG_ERROR("登録されていないカメラの削除が要求されました。シーン内に複数のカメラオブジェクトが存在する可能性があります。");
				return false;
			});
		return std::move(mgr_cnc);
	}
}
