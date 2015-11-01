#pragma once

#include <map>
#include <memory>
#include "Object.h"
#include "IUIManagerAccessor.h"
#include "IUIManagerSetup.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace core {
		class ISceneAccessForUI;
	}
	namespace game {
		class UIObject;
		class IGameProcessManagerAccessor;
		class UIManager final: public core::Object ,public IUIManagerAccessor,public IUIManagerSetup{
		public:
			void SetManagerPointer(const utility::WeakPointer<core::ISceneAccessForUI>& scene) { scene_ = scene; }
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
			bool RemoveLayer(int layer);
			/*UIオブジェクトを作成*/
			template<class C>
			std::shared_ptr<C> CreateUIObject(int layer) {
				auto uo = std::make_shared<C>();
				layers_[layer]->AddUIObject(uo);
				return uo;
			}
			
		private:
			utility::WeakPointer<core::ISceneAccessForUI> scene_;
			class Layer_ {
			public:
				void AddUIObject(const std::shared_ptr<UIObject>& o);
				void Update();
				void Draw();
			private:

			};
			std::map<int, std::unique_ptr<Layer_>> layers_;
		};
	}
}