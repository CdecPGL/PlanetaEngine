#include <ranges>

#include "StandardDebugManager.hpp"
#include "game.hpp"
#include "i_input_manager.hpp"
#include "rendering_manager.hpp"
#include "screen_drawer_2d.hpp"
#include "screen_drawer_gui.hpp"

namespace plnt::private_ {
	//////////////////////////////////////////////////////////////////////////
	//StandardDebugInformationAdder
	//////////////////////////////////////////////////////////////////////////
	class standard_debug_information_adder final : public i_debug_information_adder {
	public:
		explicit standard_debug_information_adder(screen_drawer_gui &screen_drawer_gui) : screen_drawer_gui_(
			screen_drawer_gui) {}

		void add_line(const std::string line) override {
			screen_drawer_gui_.draw_string_by_default_font({0, 16 * line_count_}, {1, 1}, line, color::white(),
			                                               color::black());
			++line_count_;
		}

	private:
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
		screen_drawer_gui &screen_drawer_gui_;
		int line_count_ = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	//StandardDebugDrawer
	//////////////////////////////////////////////////////////////////////////
	class standard_debug_drawer final : public i_debug_drawer {
	public:
		explicit standard_debug_drawer(screen_drawer_2d &screen_drawer_2d) : screen_drawer_2d_(screen_drawer_2d) {}

		void draw_line(const vector_2df &s_pos, const vector_2df &e_pos, const color &color) override {
			screen_drawer_2d_.draw_wire({s_pos, e_pos}, 1, color);
		}

		void draw_circle(const vector_2df &pos, const float radius, const color &color, bool filled) override {
			screen_drawer_2d_.draw_circle(pos, radius, color);
		}

		void draw_lines(const std::vector<vector_2df> &pos_list, const color &color) override {
			screen_drawer_2d_.draw_wire(pos_list, 1, color);
		}

	private:
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
		screen_drawer_2d &screen_drawer_2d_;
	};

	//////////////////////////////////////////////////////////////////////////
	//StandardDebugManager::Impl_
	//////////////////////////////////////////////////////////////////////////

	class standard_debug_manager::impl {
	public:
		using debug_information_map_type = std::unordered_map<
			std::string, std::function<void(i_debug_information_adder &)>>;
		using debug_draw_map_type = std::unordered_map<std::string, std::function<void(i_debug_drawer &)>>;
		debug_information_map_type debug_information_map;
		debug_draw_map_type debug_draws;
		bool is_debug_information_showing = false;
		bool enable_debug_draw = false;
		debug_information_map_type::iterator showing_debug_information_iterator;
		std::shared_ptr<screen> debug_draw_screen;
		std::shared_ptr<screen> debug_info_screen;

		void switch_showing_debug_information() {
			if (showing_debug_information_iterator != debug_information_map.end()) {
				++showing_debug_information_iterator;
			}
			if (showing_debug_information_iterator == debug_information_map.end()) {
				showing_debug_information_iterator = debug_information_map.begin();
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//StandardDebugManager
	//////////////////////////////////////////////////////////////////////////

	standard_debug_manager::standard_debug_manager() : impl_(std::make_unique<impl>()) {}

	standard_debug_manager::~standard_debug_manager() = default;

	bool standard_debug_manager::initialize(rendering_manager &rendering_mgr) {
		impl_->debug_draw_screen = rendering_mgr.get_debug_draw_screen();
		impl_->debug_info_screen = rendering_mgr.get_debug_information_screen();
		//表示中のデバッグ情報を初期化
		impl_->showing_debug_information_iterator = impl_->debug_information_map.begin();
		return true;
	}

	void standard_debug_manager::finalize() {}

	void standard_debug_manager::pre_rendering_update() {
		const auto &input_mgr = *game::instance().input_manager();
		//F1でデバッグ情報の表示有無を切り替え
		if (input_mgr.key_push(keyboard::f1)) {
			impl_->is_debug_information_showing = !impl_->is_debug_information_showing;
		}
		//デバッグ情報表示
		if (impl_->is_debug_information_showing) {
			//F2でデバッグ表示する情報切り替え
			if (input_mgr.key_push(keyboard::f2)) { impl_->switch_showing_debug_information(); }
			screen_drawer_gui scr_drawer_gui{*impl_->debug_info_screen};
			standard_debug_information_adder info_adder{scr_drawer_gui};
			//デバッグ情報があったら表示
			if (impl_->showing_debug_information_iterator != impl_->debug_information_map.end()) {
				info_adder.add_line_v("=====<", impl_->showing_debug_information_iterator->first, ">=====");
				impl_->showing_debug_information_iterator->second(info_adder);
			} else {
				info_adder.add_line_v("<デバッグ情報は存在しません>");
				info_adder.add_line_v("F2で切り替え試行");
			}
		}
		//F3でデバッグ描画の表示有無を切り替え
		if (input_mgr.key_push(keyboard::f3)) { impl_->enable_debug_draw = !impl_->enable_debug_draw; }
		//デバッグ描画
		if (impl_->enable_debug_draw) {
			screen_drawer_2d scr_drawer_2d{*impl_->debug_draw_screen};
			standard_debug_drawer drawer{scr_drawer_2d};
			for (auto &info : impl_->debug_draws | std::views::values) { info(drawer); }
		}
	}

	void standard_debug_manager::post_rendering_update() {}

	bool standard_debug_manager::create_debug_information_channel(const std::string &channel_id,
	                                                            const std::function<void(i_debug_information_adder &)> &
	                                                            handler) {
		if (impl_->debug_information_map.contains(channel_id)) { return false; }
		impl_->debug_information_map.emplace(channel_id, handler);
		return true;
	}

	bool standard_debug_manager::delete_debug_information_channel(const std::string &channel_id) {
		const auto it = impl_->debug_information_map.find(channel_id);
		if (it == impl_->debug_information_map.end()) { return false; }
		//現在表示中のデバッグ情報だったら切り替える
		if (impl_->showing_debug_information_iterator == it) { impl_->switch_showing_debug_information(); }
		impl_->debug_information_map.erase(it);
		return true;
	}

	bool standard_debug_manager::create_debug_draw_channel(const std::string &channel_id,
	                                                     const std::function<void(i_debug_drawer &)> handler) {
		if (impl_->debug_draws.contains(channel_id)) { return false; }
		impl_->debug_draws.emplace(channel_id, handler);
		return true;
	}

	bool standard_debug_manager::delete_debug_draw_channel(const std::string &channel_id) {
		const auto it = impl_->debug_draws.find(channel_id);
		if (it == impl_->debug_draws.end()) { return false; }
		impl_->debug_draws.erase(it);
		return true;
	}
}
