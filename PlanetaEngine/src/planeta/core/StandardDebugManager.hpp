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
			bool create_debug_information_channel(const std::string &channel_id,
			                                   const std::function<void(i_debug_information_adder &)> &handler) override;
			bool delete_debug_information_channel(const std::string &channel_id) override;
			bool create_debug_draw_channel(const std::string &channel_id,
			                            const std::function<void(i_debug_drawer &)> handler) override;
			bool delete_debug_draw_channel(const std::string &channel_id) override;

		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	};
}
