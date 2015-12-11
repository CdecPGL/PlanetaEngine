#pragma once

namespace planeta_engine {
	class Camera;
	namespace game {
		class GameObjectManagerPublicInterface;
		class UIManagerPublicInterface;
		class GameProcessManagerPublicInterface;
	}
	namespace core {
		class ScenePublicInterface {
		public:
			virtual ~ScenePublicInterface() = default;
			/*プロセスマネージャのインスタンスを取得*/
			virtual game::GameProcessManagerPublicInterface& game_process_manager() = 0;
			/*ゲームオブジェクトマネージャのインスタンスを取得*/
			virtual game::GameObjectManagerPublicInterface& game_object_manager() = 0;
			/*UIマネージャのインスタンスを取得*/
			virtual game::UIManagerPublicInterface& ui_manager() = 0;
			/*カメラのインスタンス取得*/
			virtual Camera& camera() = 0;
		};
	}
}