#include <functional>

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
		//////////////////////////////////////////////////////////////////////////
		//ManagerConnections
		//////////////////////////////////////////////////////////////////////////
		//CDraw用
		template<class ComType, class ManagerConnectionType>
		class StandardManagerConnection final: public ManagerConnectionType {
		public:
			StandardManagerConnection(StandardDrawSystem::ComHolder<ComType>& com_holder, typename StandardDrawSystem::ComMapType<ComType>::iterator com_map_itr)
				:com_holder_(com_holder), com_map_itr_(com_map_itr) {}

			bool Activte() override {
				return com_holder_.Activate(com_map_itr_);
			}

			bool Inactivate() override {
				return com_holder_.Inactivate(com_map_itr_);
			}

			bool Remove() override {
				return com_holder_.Remove(com_map_itr_);
			}

			bool ChangePriority(int priority) override {
				return com_holder_.ChangePriority(com_map_itr_, priority);
			}
		private:
			StandardDrawSystem::ComHolder<ComType>& com_holder_;
			typename StandardDrawSystem::ComMapType<ComType>::iterator com_map_itr_;
		};

		using StandardCDraw2DManagerConnection = StandardManagerConnection<CDraw2D, CDraw2DManagerConnection>;
		using StandardCDrawGUIManagerConnection = StandardManagerConnection<CDrawGUI, CDrawGUIManagerConnection>;
		//CCamera2D用
		class StandardCCamera2DManagerConnection : public CCamera2DManagerConnection {
		public:
			StandardCCamera2DManagerConnection(std::function<bool()>&& remove_handler)
				:remove_handler_(remove_handler) {};
			virtual bool Remove() override {
				return remove_handler_();
			}
		private:
			std::function<bool()> remove_handler_;
		};

		//////////////////////////////////////////////////////////////////////////
		//StandardDarwSystem
		//////////////////////////////////////////////////////////////////////////

		StandardDrawSystem::StandardDrawSystem() = default;

		StandardDrawSystem::~StandardDrawSystem() = default;

		void StandardDrawSystem::ExcuteDraw()
		{
			//2DDraw
			cdraw2d_holder_.Iterate([&sd_2d = *screen_drawer_2d_](CDraw2D& com){com.Draw(sd_2d); });
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

		void StandardDrawSystem::DebugInformationAddHandle(IDebugInformationAdder& di_adder) {
			di_adder.AddLine("-----GameObjectDrawSystem-----");
			di_adder.AddLineV("2D描画コンポーネント(有効数/総数):", cdraw2d_holder_.active_count(), "/", cdraw2d_holder_.all_count());
			di_adder.AddLineV("GUI描画コンポーネント(有効数/総数):", cdrawgui_holder_.active_count(), "/", cdrawgui_holder_.all_count());
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

		std::unique_ptr<planeta::private_::CDraw2DManagerConnection> StandardDrawSystem::RegisterCDraw2D(const std::shared_ptr<CDraw2D>& draw_component, int priority) {
			//コンポーネントを登録
			auto reg_res = cdraw2d_holder_.Register(draw_component, priority);
			if (!reg_res.first) { return nullptr; }
			//登録できていたらマネージャConnectionを作成し返す
			return std::make_unique<StandardCDraw2DManagerConnection>(cdraw2d_holder_, reg_res.second);
		}

		std::unique_ptr<planeta::private_::CDrawGUIManagerConnection> StandardDrawSystem::RegisterCDrawGUI(const std::shared_ptr<CDrawGUI>& draw_component, int priority) {
			//コンポーネントを登録
			auto reg_res = cdrawgui_holder_.Register(draw_component, priority);
			if (!reg_res.first) { return nullptr; }
			//登録できていたらマネージャConnectionを作成し返す
			return std::make_unique<StandardCDrawGUIManagerConnection>(cdrawgui_holder_, reg_res.second);
		}

		std::unique_ptr<planeta::private_::CCamera2DManagerConnection> StandardDrawSystem::RegisterCCamera2D(const std::shared_ptr<CCamera2D>& camera_component) {
			if (camera2d_) {
				PE_LOG_ERROR("シーン内の複数カメラはサポートされていません。初めに登録されたカメラのみが有効です。カメラコンポーネントを持つゲームオブジェクトが複数存在する可能性があります。");
				return nullptr;
			} else {
				camera2d_ = camera_component;
				//Connectionを作成して返す
				auto mgr_cnc = std::make_unique<StandardCCamera2DManagerConnection>(
					[this,ccam_ptr = camera_component.get()] {
					if (camera2d_ != nullptr && camera2d_.get() == ccam_ptr) {
						camera2d_.reset();
						return true;
					} else {
						PE_LOG_ERROR("登録されていないカメラの削除が要求されました。シーン内に複数のカメラオブジェクトが存在する可能性があります。");
						return false;
					}
				});
				return std::move(mgr_cnc);
			}
		}
	}
}
