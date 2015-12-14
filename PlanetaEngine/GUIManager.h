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
			bool RemoveGUIObject(int layer,GUIObject* uo);
			/*UIObject��\��*/
			bool ShowGUIObject(int layer, GUIObject* uo);
			/*UI�I�u�W�F�N�g�����*/
			bool CloseGUIObject(int layer, GUIObject* uo);
		private:
			GUIManager(const GUIManager&) = delete;
			GUIManager(GUIManager&&) = delete;
			GUIManager& operator=(const GUIManager&) = delete;
			GUIManager& operator=(GUIManager&&) = delete;
			std::shared_ptr<core::SceneAccessorForGUI> scene_accessor_;
			class Layer_ {
			public:
				void AddGUIObject(const std::shared_ptr<GUIObject>& o);
				void Update();
				void Draw();
				void DebugDraw();
				bool RemoveGUIObject(GUIObject* uo);
				bool ShowGUIObject(GUIObject* uo);
				bool CloseUIObject(GUIObject* uo);
			private:
				std::unordered_map<GUIObject*, std::shared_ptr<GUIObject>> active_gui_objects_;
				std::unordered_map<GUIObject*, std::shared_ptr<GUIObject>> inactive_gui_objects_;
			};
			std::map<int, Layer_> layers_;

			std::shared_ptr<GUIObject> CreateGUIObject(const std::function<std::shared_ptr<GUIObject>()>& creator, int layer)override;
		};
	}
}