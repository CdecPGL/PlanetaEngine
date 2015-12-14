#pragma once

#include <map>
#include <unordered_map>
#include <memory>
#include "SceneModule.h"
#include "WeakPointer.h"
#include "GUIManagerPublicInterface.h"

namespace planeta_engine{
	namespace core {
		class ScenePublicInterface;
		struct SceneData;
		class SceneAccessorForGUI;
	}
	namespace game {
		class GUIObject;
		class IGameProcessManagerAccessor;
		class GUIManager final: public core::SceneModule
			,public GUIManagerPublicInterface {
		public:
			GUIManager();
			/*初期化処理*/
			bool Initialize()override;
			/*終了処理*/
			void Finalize()override { return; }
			/*シーンインターフェイスをセット*/
			void SetSceneInterface(core::ScenePublicInterface& spi)override;
			/*シーンデータをセット*/
			void SetSceneData(const core::SceneData& scene_data)override;

			/*更新*/
			void Update();
			/*描画*/
			void Draw();
			/*デバッグ情報描画*/
			void DebugDraw();
			/*管理処理*/
			bool Process();
			/*レイヤーを削除*/
			bool RemoveLayer(int layer)override;
			/*UIObjectを削除*/
			bool RemoveUIObject(int layer,GUIObject* uo);
			/*UIObjectを表示*/
			bool ShowUIObject(int layer, GUIObject* uo);
			/*UIオブジェクトを閉じる*/
			bool CloseUIObject(int layer, GUIObject* uo);
		private:
			GUIManager(const GUIManager&) = delete;
			GUIManager(GUIManager&&) = delete;
			GUIManager& operator=(const GUIManager&) = delete;
			GUIManager& operator=(GUIManager&&) = delete;
			std::shared_ptr<core::SceneAccessorForGUI> scene_accessor_;
			class Layer_ {
			public:
				void AddUIObject(const std::shared_ptr<GUIObject>& o);
				void Update();
				void Draw();
				void DebugDraw();
				bool RemoveUIObject(GUIObject* uo);
				bool ShowUIObject(GUIObject* uo);
				bool CloseUIObject(GUIObject* uo);
			private:
				std::unordered_map<GUIObject*, std::shared_ptr<GUIObject>> active_ui_objects_;
				std::unordered_map<GUIObject*, std::shared_ptr<GUIObject>> inactive_ui_objects_;
			};
			std::map<int, Layer_> layers_;

			std::shared_ptr<GUIObject> CreateUIObject(const std::function<std::shared_ptr<GUIObject>()>& creator, int layer)override;
		};
	}
}