#pragma once

#include <map>
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
			/*管理処理*/
			bool Process();
			/*レイヤーを削除*/
			bool RemoveLayer(int layer)override;
			/*UIオブジェクトを作成*/
			template<class C>
			std::shared_ptr<C> CreateUIObject(int layer) {
				auto uo = std::make_shared<C>();
				layers_[layer]->AddUIObject(uo);
				return uo;
			}
			
		private:
			std::shared_ptr<core::SceneAccessorForUI> scene_accessor_;
			class Layer_ {
			public:
				void AddUIObject(const std::shared_ptr<UIObject>& o);
				void Update();
				void Draw();
			private:

			};
			std::map<int, std::unique_ptr<Layer_>> layers_;

			std::shared_ptr<UIObject> CreateUIObject(const std::function<std::shared_ptr<UIObject>()>& creator, int layer)override {
				auto nuo = creator();
				layers_[layer]->AddUIObject(nuo);
				return nuo;
			}
		};
	}
}