#pragma once

#include "i_draw_system.hpp"
#include "scene_module.hpp"

namespace plnt {
	class CDraw2D;
	class CDrawGUI;
	class CCamera2D;

	namespace private_ {
		// TODO: コンポーネント名名変更
		class CDraw2DManagerConnection {
		public:
			virtual ~CDraw2DManagerConnection() = 0 { };
			virtual bool active() = 0;
			virtual bool inactivate() = 0;
			virtual bool remove() = 0;
			virtual bool change_priority(int priority) = 0;
		};

		// TODO: コンポーネント名名変更
		class CDrawGUIManagerConnection {
		public:
			virtual ~CDrawGUIManagerConnection() = 0 { };
			virtual bool active() = 0;
			virtual bool inactivate() = 0;
			virtual bool remove() = 0;
			virtual bool change_priority(int priority) = 0;
		};

		// TODO: コンポーネント名名変更
		class CCamera2DManagerConnection {
		public:
			virtual ~CCamera2DManagerConnection() = 0 { };
			virtual bool remove() = 0;
		};

		class draw_system : public i_draw_system, public scene_module {
		public:
			draw_system() = default;
			draw_system(const draw_system &) = delete;
			draw_system(draw_system &&) = delete;
			// NOLINTNEXTLINE(modernize-use-equals-default, clang-diagnostic-microsoft-pure-definition)
			~draw_system() override = 0 { }
			draw_system &operator=(const draw_system &) = delete;
			draw_system &operator=(draw_system &&) = delete;

			/*ゲームオブジェクトの描画*/
			virtual void execute_draw() = 0;
			/*GUIの描画*/
			virtual void execute_draw_gui() = 0;
			/*カメラ状態の適用*/
			virtual void apply_camera_state() = 0;
			/*2D描画コンポーネント登録*/
			virtual std::unique_ptr<CDraw2DManagerConnection> register_c_draw_2d(
				const std::shared_ptr<CDraw2D> &draw_component, int priority) = 0;
			/*GUI描画コンポーネント登録*/
			virtual std::unique_ptr<CDrawGUIManagerConnection> register_c_draw_gui(
				const std::shared_ptr<CDrawGUI> &draw_component, int priority) = 0;
			/*カメラコンポーネント登録*/
			virtual std::unique_ptr<CCamera2DManagerConnection> register_c_camera_2d(
				const std::shared_ptr<CCamera2D> &camera_component) = 0;
		};
	}
}
