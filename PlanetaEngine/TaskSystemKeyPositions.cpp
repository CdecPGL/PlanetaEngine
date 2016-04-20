#include "TaskSystemKeyPositions.h"

namespace planeta_engine {
	namespace task_sys_key_pos {
		const char* GameObjectUpdatetTaskKeyPositionID("sys_gou"); //�Q�[���I�u�W�F�N�g�X�V�v���Z�X�D��x
		const char* TransformApplyVelocityTaskKeyPositionID("sys_tav"); //�g�����X�t�H�[�����x�K�p�v���Z�X�D��x
		//const int TransformConvertLocalToGlobalProcessPriority(200); //�g�����X�t�H�[�����[�J���O���[�o���ϊ��v���Z�X�D��x
		const char* CollisionDetectTaskKeyPositionID("sys_cd"); //�Փ˔���v���Z�X�D��x
		//const int TransformConvertGlobalToLocalProcessPriority(400);
		const char* GameObjectDrawTaskKeyPositionID("sys_god"); //�Q�[���I�u�W�F�N�g�`��v���Z�X�D��x
		const char* GUIUpdateTaskKeyPositionID("sys_gu"); //UI�X�V�v���Z�X�D��x
		const char* GUIDrawTaskKeyPositionID("sys_gd"); //UI�`��v���Z�X�D��x
	}
}