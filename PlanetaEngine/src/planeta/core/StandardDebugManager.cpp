#include "StandardDebugManager.hpp"
#include "game.hpp"
#include "i_input_manager.hpp"
#include "rendering_manager.hpp"
#include "log_utility.hpp"
#include "screen_drawer_2d.hpp"
#include "screen_drawer_gui.hpp"

namespace plnt {
	namespace private_ {
		//////////////////////////////////////////////////////////////////////////
		//StandardDebugInformationAdder
		//////////////////////////////////////////////////////////////////////////
		class StandardDebugInformationAdder : public i_debug_information_adder {
		public:
			StandardDebugInformationAdder(screen_drawer_gui &screen_drawer_gui) : screen_drawer_gui_(
				screen_drawer_gui) { };

			virtual void add_line(const std::string line) override {
				screen_drawer_gui_.draw_string_by_default_font({0, 16 * line_count_}, {1, 1}, line, color::white(),
				                                           color::black());
				++line_count_;
			}

		private:
			screen_drawer_gui &screen_drawer_gui_;
			int line_count_ = 0;
		};

		//////////////////////////////////////////////////////////////////////////
		//StandardDebugDrawer
		//////////////////////////////////////////////////////////////////////////
		class StandardDebugDrawer : public i_debug_drawer {
		public:
			StandardDebugDrawer(screen_drawer_2d &screen_drawer_2d) : screen_drawer_2d_(screen_drawer_2d) { };

			virtual void draw_line(const vector_2df &spos, const vector_2df &epos, const color &color) override {
				screen_drawer_2d_.draw_wire({spos, epos}, 1, color);
			}

			virtual void draw_circle(const vector_2df &pos, float radius, const color &color, bool filled) override {
				screen_drawer_2d_.draw_circle(pos, radius, color);
			}

			virtual void draw_lines(const std::vector<vector_2df> &pos_list, const color &color) override {
				screen_drawer_2d_.draw_wire(pos_list, 1, color);
			}

		private:
			screen_drawer_2d &screen_drawer_2d_;
		};

		//////////////////////////////////////////////////////////////////////////
		//StandardDebugManager::Impl_
		//////////////////////////////////////////////////////////////////////////

		class StandardDebugManager::Impl_ {
		public:
			using DebugInformationMapType = std::unordered_map<
				std::string, std::function<void(i_debug_information_adder &)>>;
			using DebugDrawMapType = std::unordered_map<std::string, std::function<void(i_debug_drawer &)>>;
			DebugInformationMapType debug_informations;
			DebugDrawMapType debug_draws;
			bool is_debug_information_showing = false;
			bool enable_debug_draw = false;
			DebugInformationMapType::iterator showing_debug_information_iterator;
			std::shared_ptr<screen> debug_draw_screen;
			std::shared_ptr<screen> debug_info_screen;

			void SwitchShowingDebugInformation() {
				if (showing_debug_information_iterator != debug_informations.end()) {
					++showing_debug_information_iterator;
				}
				if (showing_debug_information_iterator == debug_informations.end()) {
					showing_debug_information_iterator = debug_informations.begin();
				}
			}
		};

		//////////////////////////////////////////////////////////////////////////
		//StandardDebugManager
		//////////////////////////////////////////////////////////////////////////

		StandardDebugManager::StandardDebugManager() : impl_(std::make_unique<Impl_>()) { }

		StandardDebugManager::~StandardDebugManager() = default;

		bool StandardDebugManager::initialize(rendering_manager &rendering_mgr) {
			impl_->debug_draw_screen = rendering_mgr.get_debug_draw_screen();
			impl_->debug_info_screen = rendering_mgr.get_debug_information_screen();
			//表示中のデバッグ情報を初期化
			impl_->showing_debug_information_iterator = impl_->debug_informations.begin();
			return true;
		}

		void StandardDebugManager::finalize() { }

		void StandardDebugManager::pre_rendering_update() {
			auto &inp_mgr = *game::instance().input_manager();
			//F1でデバッグ情報の表示有無を切り替え
			if (inp_mgr.key_push(keyboard::f1)) {
				impl_->is_debug_information_showing = !impl_->is_debug_information_showing;
			}
			//デバッグ情報表示
			if (impl_->is_debug_information_showing) {
				//F2でデバッグ表示する情報切り替え
				if (inp_mgr.key_push(keyboard::f2)) { impl_->SwitchShowingDebugInformation(); }
				screen_drawer_gui scr_drawer_gui{*impl_->debug_info_screen};
				StandardDebugInformationAdder info_adder{scr_drawer_gui};
				//デバッグ情報があったら表示
				if (impl_->showing_debug_information_iterator != impl_->debug_informations.end()) {
					info_adder.add_line_v("=====<", impl_->showing_debug_information_iterator->first, ">=====");
					(impl_->showing_debug_information_iterator->second)(info_adder);
				} else {
					info_adder.add_line_v("<デバッグ情報は存在しません>");
					info_adder.add_line_v("F2で切り替え試行");
				}
			}
			//F3でデバッグ描画の表示有無を切り替え
			if (inp_mgr.key_push(keyboard::f3)) { impl_->enable_debug_draw = !impl_->enable_debug_draw; }
			//デバッグ描画
			if (impl_->enable_debug_draw) {
				screen_drawer_2d scr_drawer_2d{*impl_->debug_draw_screen};
				StandardDebugDrawer drawer{scr_drawer_2d};
				for (auto &&dd : impl_->debug_draws) { (dd.second)(drawer); }
			}
		}

		void StandardDebugManager::post_rendering_update() { }

		bool StandardDebugManager::create_debug_information_channel(const std::string &channel_id,
		                                                         const std::function<void(i_debug_information_adder &)> &
		                                                         handler) {
			if (impl_->debug_informations.find(channel_id) != impl_->debug_informations.end()) { return false; }
			impl_->debug_informations.emplace(channel_id, handler);
			return true;
		}

		bool StandardDebugManager::delete_debug_information_channel(const std::string &channel_id) {
			auto it = impl_->debug_informations.find(channel_id);
			if (it == impl_->debug_informations.end()) { return false; }
			//現在表示中のデバッグ情報だったら切り替える
			if (impl_->showing_debug_information_iterator == it) { impl_->SwitchShowingDebugInformation(); }
			impl_->debug_informations.erase(it);
			return true;
		}

		bool StandardDebugManager::create_debug_draw_channel(const std::string &channel_id,
		                                                  const std::function<void(i_debug_drawer &)> handler) {
			if (impl_->debug_draws.find(channel_id) != impl_->debug_draws.end()) { return false; }
			impl_->debug_draws.emplace(channel_id, handler);
			return true;
		}

		bool StandardDebugManager::delete_debug_draw_channel(const std::string &channel_id) {
			auto it = impl_->debug_draws.find(channel_id);
			if (it == impl_->debug_draws.end()) { return false; }
			impl_->debug_draws.erase(it);
			return true;
		}
	}
}
