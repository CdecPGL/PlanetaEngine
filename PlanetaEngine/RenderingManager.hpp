#pragma once

#include "IRenderingManager.h"
#include "SubSystemManager.h"

namespace planeta {
	namespace private_ {
		class Screen;
		/*レンダリングマネージャ*/
		class RenderingManager : public IRenderingManager, public SubSystemManager {
		public:
			virtual ~RenderingManager()override = default;
			/*初期化する*/
			virtual bool Initialize() = 0;
			/*終了処理を行う*/
			virtual void Finalize() = 0;
			/*更新する*/
			virtual bool Update() = 0;
			/*デバッグ描画用スクリーンを取得する*/
			virtual std::shared_ptr<private_::Screen> GetDebugDrawScreen() = 0;
			/*デバッグ情報描画用スクリーンを取得する*/
			virtual std::shared_ptr<private_::Screen> GetDebugInformationScreen() = 0;
		};
	}
}