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
	namespace private_ {
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
						PE_LOG_ERROR("DrawComponent::Drawで無効なWeakPointerが参照されました。問題の発生したコンポーネントはリストから除外されます。", e.what());
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
				PE_LOG_ERROR("シーン内の複数カメラはサポートされていません。初めに登録されたカメラのみが有効です。カメラコンポーネントを持つゲームオブジェクトが複数存在する可能性があります。");
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
				PE_LOG_ERROR("登録されていないカメラの削除が要求されました。シーン内に複数のカメラオブジェクトが存在する可能性があります。");
			}
		}

		void GameObjectDrawSystem::ApplyCameraState() {
			if (camera2d_) {
				CTransform2D& trans = camera2d_->transform2d();
				double scale = camera2d_->expansion();
				double rota_rad = trans.rotation_rad();
				Vector2Dd pos = trans.position();
				SetupCamera_Ortho((float)(private_::engine_config::engine::DrawSize().y / scale));
				SetCameraPositionAndAngle(VGet((float)pos.x, (float)pos.y, GetCameraPosition().z), GetCameraAngleVRotate(), GetCameraAngleHRotate(), (float)rota_rad);
			} else {
				PE_LOG_WARNING("シーン内にカメラ2Dが登録されていません。");
			}
		}

	}
}
