#include "StandardDebugManager.h"
#include "RenderingManager.h"
#include "LogUtility.h"
#include "ScreenDrawer2D.h"

namespace planeta {
	namespace private_ {
		//////////////////////////////////////////////////////////////////////////
		//StandardDebugInformationAdder
		//////////////////////////////////////////////////////////////////////////
		class StandardDebugInformationAdder : public IDebugInformationAdder{
		public:
			virtual void AddLine(const std::string line) override {
				throw std::logic_error("The method or operation is not implemented.");
			}

		};

		//////////////////////////////////////////////////////////////////////////
		//StandardDebugDrawer
		//////////////////////////////////////////////////////////////////////////
		class StandardDebugDrawer : public IDebugDrawer{
		public:
			StandardDebugDrawer(ScreenDrawer2D& screen_drawer_2d) :screen_drawer_2d_(screen_drawer_2d) {};

			virtual void DrawLine(const Vector2Df& spos, const Vector2Df& epos, const Color& color) override {
				screen_drawer_2d_.DrawWire({spos, epos}, 1, color);
			}

			virtual void DrawCircle(const Vector2Df& pos, float radius, const Color& color, bool filled) override {
				screen_drawer_2d_.DrawCircle(pos, radius, color);
			}

			virtual void DrawLines(const std::vector<Vector2Df>& pos_list, const Color& color) override {
				screen_drawer_2d_.DrawWire(pos_list, 1, color);
			}
		private:
			ScreenDrawer2D screen_drawer_2d_;
		};

		//////////////////////////////////////////////////////////////////////////
		//StandardDebugManager::Impl_
		//////////////////////////////////////////////////////////////////////////

		class StandardDebugManager::Impl_ {
		public:
			using DebugInformationMapType = std::unordered_map<std::string, std::function<void(IDebugInformationAdder&)>>;
			using DebugDrawMapType = std::unordered_map<std::string, std::function<void(IDebugDrawer&)>>;
			DebugInformationMapType debug_informations;
			DebugDrawMapType debug_draws;
			bool is_debug_information_showing;
			DebugInformationMapType::iterator showing_debug_information_iterator;
			std::unordered_map<std::string, DebugDrawMapType::iterator> showing_debug_draw_iterator_map;
			std::shared_ptr<Screen> debug_draw_screen;
			std::shared_ptr<Screen> debug_info_screen;
		};

		//////////////////////////////////////////////////////////////////////////
		//StandardDebugManager
		//////////////////////////////////////////////////////////////////////////

		StandardDebugManager::StandardDebugManager() :impl_(std::make_unique<Impl_>()) {}
		StandardDebugManager::~StandardDebugManager() = default;

		bool StandardDebugManager::Initialize(RenderingManager& rendering_mgr) {
			impl_->debug_draw_screen = rendering_mgr.GetDebugDrawScreen();
			impl_->debug_info_screen = rendering_mgr.GetDebugInformationScreen();
			return true;
		}

		void StandardDebugManager::Finalize() {
			
		}

		void StandardDebugManager::Update() {
			StandardDebugInformationAdder info_adder;
			if (impl_->is_debug_information_showing) {
				(impl_->showing_debug_information_iterator->second)(info_adder);
			}
			ScreenDrawer2D scr_drawer_2d{ *impl_->debug_draw_screen };
			StandardDebugDrawer drawer{scr_drawer_2d};
			for (auto&& addi : impl_->showing_debug_draw_iterator_map) {
				(addi.second->second)(drawer);
			}
		}

		bool StandardDebugManager::CreateDebugInformationChannel(const std::string& channel_id, const std::function<void(IDebugInformationAdder &) >& handler) {
			if (impl_->debug_informations.find(channel_id) != impl_->debug_informations.end()) { return false; }
			impl_->debug_informations.emplace(channel_id, handler);
			return true;
		}

		bool StandardDebugManager::DeleteDebugInformationChannel(const std::string& channel_id) {
			auto it = impl_->debug_informations.find(channel_id);
			if (it == impl_->debug_informations.end()) { return false; }
			impl_->debug_informations.erase(it);
			return true;
		}

		bool StandardDebugManager::CreateDebugDrawChannel(const std::string& channel_id, const std::function<void(IDebugDrawer &) > handler) {
			if (impl_->debug_draws.find(channel_id) != impl_->debug_draws.end()) { return false; }
			impl_->debug_draws.emplace(channel_id, handler);
			return true;
		}

		bool StandardDebugManager::DeleteDebugDrawChannel(const std::string& channel_id) {
			auto it = impl_->debug_draws.find(channel_id);
			if (it == impl_->debug_draws.end()) { return false; }
			impl_->debug_draws.erase(it);
			return true;
		}
	}
}