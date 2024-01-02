#pragma once

#include <memory>

#include "i_scene_manager.hpp"
#include "sub_system_manager.hpp"

namespace plnt {
	class collision_group_matrix;

	namespace private_ {
		class resource_manager;

		/*シーンの状況(継続、終了、エラー発生)*/
		enum class scene_status { play, quit, error };

		/*シーンマネージャ*/
		class scene_manager : public i_scene_manager, public sub_system_manager {
		public:
			scene_manager() = default;
			scene_manager(const scene_manager &) = delete;
			scene_manager(scene_manager &&) = delete;
			~scene_manager() override = default;
			scene_manager &operator=(const scene_manager &) = delete;
			scene_manager &operator=(scene_manager &&) = delete;

			/*マネージャの初期化*/
			virtual bool initialize() = 0;
			/*マネージャの終了処理*/
			virtual bool finalize() = 0;
			/*シーン処理*/
			virtual scene_status process() = 0;
			/*リソースマネージャをセット(初期化前)*/
			virtual void set_resource_manager(const std::shared_ptr<resource_manager> &mgr) = 0;
		};
	}
}
