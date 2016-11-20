#include "Game.h"
#include "RenderingManager.h"
#include "ConfigManager.h"
#include "StandardDrawSystem.h"
#include "CDraw2D.h"
#include "LogUtility.h"
#include "ScreenDrawer2D.h"
#include "ScreenDrawerGUI.h"
#include "CCamera2D.h"
#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "CTransform2D.h"
#include "Screen.h"
#include "IDebugManager.h"

namespace planeta{
	namespace private_ {
		StandardDrawSystem::StandardDrawSystem() = default;

		StandardDrawSystem::~StandardDrawSystem() = default;

		bool StandardDrawSystem::Remove(const std::shared_ptr<CDraw2D>& draw_component)
		{
			auto it = _draw_component_map.find(draw_component.get());
			if (it == _draw_component_map.end()) { return false; }
			else {
				_draw_component_update_list[it->second.first].erase(it->second.second);
				_draw_component_map.erase(it);
				return true;
			}
		}

		void StandardDrawSystem::ExcuteDraw()
		{
			//2DDraw
			for (auto& dcl : _draw_component_update_list) {
				for (auto it = dcl.second.begin(); it != dcl.second.end(); ++it) {
					try {
						(*it)->Draw(*screen_drawer_2d_);
					}
					catch (NullWeakPointerException& e) {
						PE_LOG_ERROR("DrawComponent::Drawで無効なWeakPointerが参照されました。問題の発生したコンポーネントはリストから除外されます。", e.what());
						dcl.second.erase(it);
						break;
					}
				}
			}
		}

		void StandardDrawSystem::Update() {
			
		}

		bool StandardDrawSystem::Initialize() {
			screen_ = Game::instance().rendering_manager()->GetMainScreen();
			if (!screen_) { return false; }
			screen_drawer_2d_ = std::make_unique<ScreenDrawer2D>(*screen_);
			screen_drawer_gui_ = std::make_unique<ScreenDrawerGUI>(*screen_);
			return true;
		}

		void StandardDrawSystem::Finalize() {
			return;
		}

		bool StandardDrawSystem::RegisterCamera(const std::shared_ptr<CCamera2D>& camera_component) {
			if (camera2d_) {
				PE_LOG_ERROR("シーン内の複数カメラはサポートされていません。初めに登録されたカメラのみが有効です。カメラコンポーネントを持つゲームオブジェクトが複数存在する可能性があります。");
				return false;
			} else {
				camera2d_ = camera_component;
				return true;
			}
		}

		void StandardDrawSystem::RemoveCamera(CCamera2D* camera_component) {
			if (camera2d_ != nullptr && camera2d_.get() == camera_component) {
				camera2d_.reset();
			} else {
				PE_LOG_ERROR("登録されていないカメラの削除が要求されました。シーン内に複数のカメラオブジェクトが存在する可能性があります。");
			}
		}

		void StandardDrawSystem::DebugInformationAddHandle(IDebugInformationAdder& di_adder) {
			di_adder.AddLine("-----GameObjectDrawSystem-----");
			di_adder.AddLineV("描画コンポーネント数:", _draw_component_map.size());
		}

		void StandardDrawSystem::ApplyCameraState() {
			if (camera2d_) {
				double scale = camera2d_->expansion();
				double rota_rad = camera2d_->rotation_rad();
				Vector2Dd pos = camera2d_->position();
				SetupCamera_Ortho((float)(Game::instance().config_manager()->draw_size().y / scale));
				SetCameraPositionAndAngle(VGet((float)pos.x, (float)pos.y, GetCameraPosition().z), GetCameraAngleVRotate(), GetCameraAngleHRotate(), (float)rota_rad);
				//Effekseerのカメラを同期
				Effekseer_Sync3DSetting();
			} else {
				PE_LOG_WARNING("シーン内にカメラ2Dが登録されていません。");
			}
		}

		Vector2Dd StandardDrawSystem::CovertPositionScreenSpaceToGameObjectSpace(const Vector2Di& ui_pos)const {
			VECTOR gov = ConvScreenPosToWorldPos(VGet((float)ui_pos.x, (float)ui_pos.y, 0.0f));
			return Vector2Dd(gov.x, gov.y);
		}

		Vector2Di StandardDrawSystem::CovertPositionGameObjectSpaceToScreenSpace(const Vector2Dd& game_object_pos)const {
			VECTOR uiv = ConvWorldPosToScreenPos(VGet((float)game_object_pos.x, (float)game_object_pos.y, 0.0f));
			return Vector2Di((int)uiv.x, (int)uiv.y);
		}

	}
}
