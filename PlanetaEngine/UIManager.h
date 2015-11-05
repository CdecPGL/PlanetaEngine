#pragma once

#include <map>
#include <unordered_map>
#include <memory>
#include "Object.h"
#include "WeakPointer.h"
#include "UIManagerPublicInterface.h"

namespace planeta_engine{
	namespace core {
		class ScenePublicInterface;
		class SceneAccessorForUI;
	}
	namespace game {
		class UIObject;
		class IGameProcessManagerAccessor;
		class UIManager final: public core::Object,public UIManagerPublicInterface {
		public:
			UIManager();
			/*シーンをセット*/
			void SetScene(core::ScenePublicInterface& spi);
			/*初期化処理*/
			bool Initialize();
			/*終了処理*/
			bool Finalize() { return true; }
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
			bool RemoveUIObject(int layer,UIObject* uo);
			/*UIObjectを表示*/
			bool ShowUIObject(int layer, UIObject* uo);
			/*UIオブジェクトを閉じる*/
			bool CloseUIObject(int layer, UIObject* uo);
		private:
			UIManager(const UIManager&) = delete;
			UIManager(UIManager&&) = delete;
			UIManager& operator=(const UIManager&) = delete;
			UIManager& operator=(UIManager&&) = delete;
			std::shared_ptr<core::SceneAccessorForUI> scene_accessor_;
			class Layer_ {
			public:
				void AddUIObject(const std::shared_ptr<UIObject>& o);
				void Update();
				void Draw();
				void DebugDraw();
				bool RemoveUIObject(UIObject* uo);
				bool ShowUIObject(UIObject* uo);
				bool CloseUIObject(UIObject* uo);
			private:
				std::unordered_map<UIObject*, std::shared_ptr<UIObject>> active_ui_objects_;
				std::unordered_map<UIObject*, std::shared_ptr<UIObject>> inactive_ui_objects_;
			};
			std::map<int, Layer_> layers_;

			std::shared_ptr<UIObject> CreateUIObject(const std::function<std::shared_ptr<UIObject>()>& creator, int layer)override;
		};
	}
}