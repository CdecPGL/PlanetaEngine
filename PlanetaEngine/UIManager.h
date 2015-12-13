#pragma once

#include <map>
#include <unordered_map>
#include <memory>
#include "SceneModule.h"
#include "WeakPointer.h"
#include "UIManagerPublicInterface.h"

namespace planeta_engine{
	namespace core {
		class ScenePublicInterface;
		struct SceneData;
		class SceneAccessorForUI;
	}
	namespace game {
		class UIObject;
		class IGameProcessManagerAccessor;
		class UIManager final: public core::SceneModule
			,public UIManagerPublicInterface {
		public:
			UIManager();
			/*����������*/
			bool Initialize()override;
			/*�I������*/
			void Finalize()override { return; }
			/*�V�[���C���^�[�t�F�C�X���Z�b�g*/
			void SetSceneInterface(core::ScenePublicInterface& spi)override;
			/*�V�[���f�[�^���Z�b�g*/
			void SetSceneData(const core::SceneData& scene_data)override;

			/*�X�V*/
			void Update();
			/*�`��*/
			void Draw();
			/*�f�o�b�O���`��*/
			void DebugDraw();
			/*�Ǘ�����*/
			bool Process();
			/*���C���[���폜*/
			bool RemoveLayer(int layer)override;
			/*UIObject���폜*/
			bool RemoveUIObject(int layer,UIObject* uo);
			/*UIObject��\��*/
			bool ShowUIObject(int layer, UIObject* uo);
			/*UI�I�u�W�F�N�g�����*/
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