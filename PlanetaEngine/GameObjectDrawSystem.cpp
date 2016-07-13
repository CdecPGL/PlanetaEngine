#include "GameObjectDrawSystem.h"
#include "CDraw2D.h"
#include "SystemLog.h"
#include "ScreenDrawer2D.h"
#include "ScreenDrawerGUI.h"
#include "RenderManager.h"
#include "CCamera2D.h"
#include "DxLib.h"
#include "EngineConfigData.h"
#include "CTransform2D.h"
#include "Screen.h"

namespace planeta{
	namespace core {
		GameObjectDrawSystem::GameObjectDrawSystem()
		{

		}

		bool GameObjectDrawSystem::Remove(const std::shared_ptr<CDraw2D>& draw_component)
		{
			auto it = _draw_component_map.find(draw_component.get());
			if (it == _draw_component_map.end()) { return false; }
			else {
				_draw_component_update_list[it->second.first].erase(it->second.second);
				_draw_component_map.erase(it);
				return true;
			}
		}

		void GameObjectDrawSystem::ExcuteDraw()
		{
			//2DDraw
			for (auto& dcl : _draw_component_update_list) {
				for (auto it = dcl.second.begin(); it != dcl.second.end(); ++it) {
					try {
						(*it)->Draw(*screen_drawer_2d_);
					}
					catch (util::NullWeakPointerException& e) {
						PE_LOG_ERROR("DrawComponent::Draw�Ŗ�����WeakPointer���Q�Ƃ���܂����B���̔��������R���|�[�l���g�̓��X�g���珜�O����܂��B", e.what());
						dcl.second.erase(it);
						break;
					}
				}
			}
		}

		void GameObjectDrawSystem::Update() {
			
		}

		bool GameObjectDrawSystem::Initialize() {
			screen_ = RenderManager::instance().CreateScreen();
			if (!screen_) { return false; }
			screen_drawer_2d_ = std::make_unique<ScreenDrawer2D>(*screen_);
			screen_drawer_gui_ = std::make_unique<ScreenDrawerGUI>(*screen_);
			return true;
		}

		void GameObjectDrawSystem::Finalize() {
			RenderManager::instance().DisposeScreen(screen_);
			return;
		}

		bool GameObjectDrawSystem::RegisterCamera(const std::shared_ptr<CCamera2D>& camera_component) {
			if (camera2d_) {
				PE_LOG_ERROR("�V�[�����̕����J�����̓T�|�[�g����Ă��܂���B���߂ɓo�^���ꂽ�J�����݂̂��L���ł��B�J�����R���|�[�l���g�����Q�[���I�u�W�F�N�g���������݂���\��������܂��B");
				return false;
			} else {
				camera2d_ = camera_component;
				return true;
			}
		}

		void GameObjectDrawSystem::RemoveCamera(CCamera2D* camera_component) {
			if (camera2d_ != nullptr && camera2d_.get() == camera_component) {
				camera2d_.reset();
			} else {
				PE_LOG_ERROR("�o�^����Ă��Ȃ��J�����̍폜���v������܂����B�V�[�����ɕ����̃J�����I�u�W�F�N�g�����݂���\��������܂��B");
			}
		}

		void GameObjectDrawSystem::ApplyCameraState() {
			if (camera2d_) {
				CTransform2D& trans = camera2d_->transform2d();
				double scale = camera2d_->expansion();
				double rota_rad = trans.rotation_rad();
				Vector2Dd pos = trans.position();
				SetupCamera_Ortho((float)(core::engine_config::engine::DrawSize().y / scale));
				SetCameraPositionAndAngle(VGet((float)pos.x, (float)pos.y, GetCameraPosition().z), GetCameraAngleVRotate(), GetCameraAngleHRotate(), (float)rota_rad);
			} else {
				PE_LOG_WARNING("�V�[�����ɃJ����2D���o�^����Ă��܂���B");
			}
		}

	}
}
