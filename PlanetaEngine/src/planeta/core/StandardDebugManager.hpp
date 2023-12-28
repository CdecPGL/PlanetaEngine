#pragma once

#include <memory>
#include "debug_manager.hpp"

namespace plnt {
	namespace private_ {
		/*標準のデバッグマネージャ*/
		class StandardDebugManager final : public debug_manager {
		public:
			StandardDebugManager();
			~StandardDebugManager() override;
			bool initialize(RenderingManager &rendering_mgr) override;
			void finalize() override;
			void pre_rendering_update() override;
			void post_rendering_update() override;
			bool CreateDebugInformationChannel(const std::string &channel_id,
			                                   const std::function<void(IDebugInformationAdder &)> &handler) override;
			bool DeleteDebugInformationChannel(const std::string &channel_id) override;
			bool CreateDebugDrawChannel(const std::string &channel_id,
			                            const std::function<void(IDebugDrawer &)> handler) override;
			bool DeleteDebugDrawChannel(const std::string &channel_id) override;

		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	};
}
