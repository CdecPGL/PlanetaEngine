#pragma once

#include <memory>

#include "i_scene_manager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	class collision_group_matrix;

	namespace private_ {
		class resource_manager;

		/*シーンの状況(継続、終了、エラー発生)*/
		enum class SceneStatus_ { Continue, Quit, Error };

		/*シーンマネージャ*/
		class SceneManager : public i_scene_manager, public SubSystemManager {
		public:
			virtual ~SceneManager() override = default;
			/*マネージャの初期化*/
			virtual bool Initialize() = 0;
			/*マネージャの終了処理*/
			virtual bool Finalize() = 0;
			/*シーン処理*/
			virtual SceneStatus_ Process_() = 0;
			/*リソースマネージャをセット(初期化前)*/
			virtual void SetResouceManager(const std::shared_ptr<resource_manager> &mgr) = 0;
		};
	}
}
