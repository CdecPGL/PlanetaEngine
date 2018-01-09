#pragma once

#include <memory>

#include "ISceneManager.h"
#include "SubSystemManager.h"

namespace planeta {
	class CollisionGroupMatrix;
	namespace private_ {
		class ResourceManager;
		/*シーンの状況(継続、終了、エラー発生)*/
		enum class SceneStatus_ { Continue, Quit, Error };
		/*シーンマネージャ*/
		class SceneManager : public ISceneManager, public SubSystemManager {
		public:
			virtual ~SceneManager()override = default;
			/*マネージャの初期化*/
			virtual bool Initialize() = 0;
			/*マネージャの終了処理*/
			virtual bool Finalize() = 0;
			/*シーン処理*/
			virtual SceneStatus_ Process_() = 0;
			/*リソースマネージャをセット(初期化前)*/
			virtual void SetResouceManager(const std::shared_ptr<ResourceManager>& mgr) = 0;
		};
	}
}