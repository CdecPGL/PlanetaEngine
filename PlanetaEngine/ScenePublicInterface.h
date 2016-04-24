#pragma once

namespace planeta_engine {
	class Camera;
	class GameObjectManagerPublicInterface;
	class GUIManagerPublicInterface;
	class TaskManagerPublicInterface;
	namespace core {
		class ScenePublicInterface {
		public:
			virtual ~ScenePublicInterface() = default;
			/*プロセスマネージャのインスタンスを取得*/
			virtual TaskManagerPublicInterface& game_process_manager() = 0;
			/*ゲームオブジェクトマネージャのインスタンスを取得*/
			virtual GameObjectManagerPublicInterface& game_object_manager() = 0;
			/*GUIマネージャのインスタンスを取得*/
			virtual GUIManagerPublicInterface& gui_manager() = 0;
			/*カメラのインスタンスを取得*/
			virtual Camera& camera() = 0;
		};
	}
}