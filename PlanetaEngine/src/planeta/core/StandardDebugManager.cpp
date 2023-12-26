#include "StandardDebugManager.hpp"
#include "Game.hpp"
#include "IInputManager.hpp"
#include "RenderingManager.hpp"
#include "LogUtility.hpp"
#include "ScreenDrawer2D.hpp"
#include "ScreenDrawerGUI.hpp"

namespace plnt {
	namespace private_ {
		//////////////////////////////////////////////////////////////////////////
		//StandardDebugInformationAdder
		//////////////////////////////////////////////////////////////////////////
		class StandardDebugInformationAdder : public IDebugInformationAdder {
		public:
			StandardDebugInformationAdder(ScreenDrawerGUI &screen_drawer_gui) : screen_drawer_gui_(
				screen_drawer_gui) { };

			virtual void AddLine(const std::string line) override {
				screen_drawer_gui_.DrawStringByDefaultFont({0, 16 * line_count_}, {1, 1}, line, Color::White(),
				                                           Color::Black());
				++line_count_;
			}

		private:
			ScreenDrawerGUI &screen_drawer_gui_;
			int line_count_ = 0;
		};

		//////////////////////////////////////////////////////////////////////////
		//StandardDebugDrawer
		//////////////////////////////////////////////////////////////////////////
		class StandardDebugDrawer : public IDebugDrawer {
		public:
			StandardDebugDrawer(ScreenDrawer2D &screen_drawer_2d) : screen_drawer_2d_(screen_drawer_2d) { };

			virtual void DrawLine(const Vector2Df &spos, const Vector2Df &epos, const Color &color) override {
				screen_drawer_2d_.DrawWire({spos, epos}, 1, color);
			}

			virtual void DrawCircle(const Vector2Df &pos, float radius, const Color &color, bool filled) override {
				screen_drawer_2d_.DrawCircle(pos, radius, color);
			}

			virtual void DrawLines(const std::vector<Vector2Df> &pos_list, const Color &color) override {
				screen_drawer_2d_.DrawWire(pos_list, 1, color);
			}

		private:
			ScreenDrawer2D &screen_drawer_2d_;
		};

		//////////////////////////////////////////////////////////////////////////
		//StandardDebugManager::Impl_
		//////////////////////////////////////////////////////////////////////////

		class StandardDebugManager::Impl_ {
		public:
			using DebugInformationMapType = std::unordered_map<
				std::string, std::function<void(IDebugInformationAdder &)>>;
			using DebugDrawMapType = std::unordered_map<std::string, std::function<void(IDebugDrawer &)>>;
			DebugInformationMapType debug_informations;
			DebugDrawMapType debug_draws;
			bool is_debug_information_showing = false;
			bool enable_debug_draw = false;
			DebugInformationMapType::iterator showing_debug_information_iterator;
			std::shared_ptr<Screen> debug_draw_screen;
			std::shared_ptr<Screen> debug_info_screen;

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

		bool StandardDebugManager::Initialize(RenderingManager &rendering_mgr) {
			impl_->debug_draw_screen = rendering_mgr.GetDebugDrawScreen();
			impl_->debug_info_screen = rendering_mgr.GetDebugInformationScreen();
			//表示中のデバッグ情報を初期化
			impl_->showing_debug_information_iterator = impl_->debug_informations.begin();
			return true;
		}

		void StandardDebugManager::Finalize() { }

		void StandardDebugManager::PreRenderingUpdate() {
			auto &inp_mgr = *Game::instance().input_manager();
			//F1でデバッグ情報の表示有無を切り替え
			if (inp_mgr.KeyPush(Key::F1)) {
				impl_->is_debug_information_showing = !impl_->is_debug_information_showing;
			}
			//デバッグ情報表示
			if (impl_->is_debug_information_showing) {
				//F2でデバッグ表示する情報切り替え
				if (inp_mgr.KeyPush(Key::F2)) { impl_->SwitchShowingDebugInformation(); }
				ScreenDrawerGUI scr_drawer_gui{*impl_->debug_info_screen};
				StandardDebugInformationAdder info_adder{scr_drawer_gui};
				//デバッグ情報があったら表示
				if (impl_->showing_debug_information_iterator != impl_->debug_informations.end()) {
					info_adder.AddLineV("=====<", impl_->showing_debug_information_iterator->first, ">=====");
					(impl_->showing_debug_information_iterator->second)(info_adder);
				} else {
					info_adder.AddLineV("<デバッグ情報は存在しません>");
					info_adder.AddLineV("F2で切り替え試行");
				}
			}
			//F3でデバッグ描画の表示有無を切り替え
			if (inp_mgr.KeyPush(Key::F3)) { impl_->enable_debug_draw = !impl_->enable_debug_draw; }
			//デバッグ描画
			if (impl_->enable_debug_draw) {
				ScreenDrawer2D scr_drawer_2d{*impl_->debug_draw_screen};
				StandardDebugDrawer drawer{scr_drawer_2d};
				for (auto &&dd : impl_->debug_draws) { (dd.second)(drawer); }
			}
		}

		void StandardDebugManager::PostRenderingUpdate() { }

		bool StandardDebugManager::CreateDebugInformationChannel(const std::string &channel_id,
		                                                         const std::function<void(IDebugInformationAdder &)> &
		                                                         handler) {
			if (impl_->debug_informations.find(channel_id) != impl_->debug_informations.end()) { return false; }
			impl_->debug_informations.emplace(channel_id, handler);
			return true;
		}

		bool StandardDebugManager::DeleteDebugInformationChannel(const std::string &channel_id) {
			auto it = impl_->debug_informations.find(channel_id);
			if (it == impl_->debug_informations.end()) { return false; }
			//現在表示中のデバッグ情報だったら切り替える
			if (impl_->showing_debug_information_iterator == it) { impl_->SwitchShowingDebugInformation(); }
			impl_->debug_informations.erase(it);
			return true;
		}

		bool StandardDebugManager::CreateDebugDrawChannel(const std::string &channel_id,
		                                                  const std::function<void(IDebugDrawer &)> handler) {
			if (impl_->debug_draws.find(channel_id) != impl_->debug_draws.end()) { return false; }
			impl_->debug_draws.emplace(channel_id, handler);
			return true;
		}

		bool StandardDebugManager::DeleteDebugDrawChannel(const std::string &channel_id) {
			auto it = impl_->debug_draws.find(channel_id);
			if (it == impl_->debug_draws.end()) { return false; }
			impl_->debug_draws.erase(it);
			return true;
		}
	}
}
