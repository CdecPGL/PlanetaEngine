#pragma once

#include <memory>

#include "debug_manager.hpp"

namespace plnt {
	namespace private_ {
		/*標準のデバッグマネージャ*/
		class standard_debug_manager final : public debug_manager {
		public:
			standard_debug_manager();
			standard_debug_manager(const standard_debug_manager &) = delete;
			standard_debug_manager(standard_debug_manager &&) = delete;
			~standard_debug_manager() override;
			standard_debug_manager &operator=(const standard_debug_manager &) = delete;
			standard_debug_manager &operator=(standard_debug_manager &&) = delete;

			bool initialize(rendering_manager &rendering_mgr) override;
			void finalize() override;
			void pre_rendering_update() override;
			void post_rendering_update() override;
			[[nodiscard]] bool create_debug_information_channel(const std::string &channel_id,
			                                                    const std::function<void(i_debug_information_adder &)> &
			                                                    handler) override;
			bool delete_debug_information_channel(const std::string &channel_id) override;
			[[nodiscard]] bool create_debug_draw_channel(const std::string &channel_id,
			                                             std::function<void(i_debug_drawer &)> handler) override;
			bool delete_debug_draw_channel(const std::string &channel_id) override;

		private:
			class impl;
			std::unique_ptr<impl> impl_;
		};
	};
}
