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
			/*����������*/
			bool Initialize();
			/*�I������*/
			bool Finalize() { return true; }
			/*�X�V*/
			void Update();
			/*�`��*/
			void Draw();
			/*�Ǘ�����*/
			bool Process();
			/*���C���[���폜*/
			bool RemoveLayer(int layer);
			/*UI�I�u�W�F�N�g���쐬*/
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