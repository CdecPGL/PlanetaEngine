#include "GameObjectSetUpper.h"
#include "GameObjectAccessorForSetUp.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace game {

		bool GameObjectSetUpper::operator()(GameObject& game_object) {
			if (!game_object.SetUpSystemComponent()) {
				debug::SystemLog::instance().LogError("GameObjectシステムコンポーネントの設定に失敗しました。", __FUNCTION__);
				return false;
			}
			GameObjectAccessorForSetUp gosup(game_object);
			if (!_SetUpGameObject(gosup)) {
				debug::SystemLog::instance().LogError("GameObjectの初期設定に失敗しました。", __FUNCTION__);
				return false;
			}
			if (!game_object.Initialize_()) {
				debug::SystemLog::instance().LogError("GameObjectの初期化に失敗しました。", __FUNCTION__);
				return false;
			}
			return true;
		}

	}
}