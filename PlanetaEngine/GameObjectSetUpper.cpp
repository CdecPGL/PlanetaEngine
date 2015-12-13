#include "GameObjectSetUpper.h"
#include "GameObjectAccessorForSetUp.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace game {

		bool GameObjectSetUpper::operator()(GameObject& game_object) {
			if (!game_object.SetUpSystemComponent()) {
				debug::SystemLog::instance().LogError("GameObject�V�X�e���R���|�[�l���g�̐ݒ�Ɏ��s���܂����B", __FUNCTION__);
				return false;
			}
			GameObjectAccessorForSetUp gosup(game_object);
			if (!_SetUpGameObject(gosup)) {
				debug::SystemLog::instance().LogError("GameObject�̏����ݒ�Ɏ��s���܂����B", __FUNCTION__);
				return false;
			}
			if (!game_object.Initialize_()) {
				debug::SystemLog::instance().LogError("GameObject�̏������Ɏ��s���܂����B", __FUNCTION__);
				return false;
			}
			return true;
		}

	}
}