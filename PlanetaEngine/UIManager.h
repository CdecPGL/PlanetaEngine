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
			/*�V�[�����Z�b�g*/
			void SetScene(core::ScenePublicInterface& spi);
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
			bool RemoveLayer(int layer)override;
			/*UI�I�u�W�F�N�g���쐬*/
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