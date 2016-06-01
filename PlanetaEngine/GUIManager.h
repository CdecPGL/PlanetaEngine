#pragma once

#include <map>
#include <unordered_map>
#include <memory>
#include "SceneModule.h"
#include "WeakPointer.h"
#include "GUIManagerPublicInterface.h"
#include "NonCopyable.h"

namespace planeta{
	//class ScreenDrawerGUI;
	//class SceneAccessorForGUI;
	//namespace core {
	//	class ScenePublicInterface;
	//	struct SceneData;
	//	struct SceneDataForGUI;
	//}
	//namespace game {
	//	class GUIObject;
	//	class ITaskManagerAccessor;
	//	class GUIManager final: public core::SceneModule
	//		,public GUIManagerPublicInterface ,private utility::NonCopyable<GUIManager>{
	//	public:
	//		GUIManager();
	//		~GUIManager();
	//		/*����������*/
	//		bool Initialize()override;
	//		/*�I������*/
	//		void Finalize()override { return; }
	//		/*�V�[���C���^�[�t�F�C�X���Z�b�g*/
	//		void SetSceneInterface(core::ScenePublicInterface& spi)override;
	//		/*�V�[���f�[�^���Z�b�g*/
	//		void SetSceneData(const core::SceneData& scene_data)override;

	//		/*�X�V*/
	//		void Update();
	//		/*�`��*/
	//		void Draw(ScreenDrawerGUI& drawer);
	//		/*�f�o�b�O���`��*/
	//		void DebugDraw();
	//		/*�Ǘ�����*/
	//		bool Process();
	//		/*���C���[���폜*/
	//		bool RemoveLayer(int layer)override;
	//		/*UIObject���폜*/
	//		bool RemoveGUIObject(int layer,GUIObject* uo);
	//		/*UIObject��\��*/
	//		bool ShowGUIObject(int layer, GUIObject* uo);
	//		/*UI�I�u�W�F�N�g�����*/
	//		bool CloseGUIObject(int layer, GUIObject* uo);

	//		/*�V�[���f�[�^���Q��*/
	//		core::SceneDataForGUI& RefSceneData() { return *scene_data_; }
	//	private:
	//		std::shared_ptr<SceneAccessorForGUI> scene_accessor_;
	//		std::unique_ptr<core::SceneDataForGUI> scene_data_;
	//		class Layer_ {
	//		public:
	//			void AddGUIObject(const std::shared_ptr<GUIObject>& o);
	//			void Update();
	//			void Draw(ScreenDrawerGUI& drawer);
	//			void DebugDraw();
	//			bool RemoveGUIObject(GUIObject* uo);
	//			bool ShowGUIObject(GUIObject* uo);
	//			bool CloseUIObject(GUIObject* uo);
	//		private:
	//			std::unordered_map<GUIObject*, std::shared_ptr<GUIObject>> active_gui_objects_;
	//			std::unordered_map<GUIObject*, std::shared_ptr<GUIObject>> inactive_gui_objects_;
	//		};
	//		std::map<int, Layer_> layers_;

	//		std::shared_ptr<GUIObject> CreateGUIObject(const std::function<std::shared_ptr<GUIObject>()>& creator, int layer)override;
	//	};
	//}
}