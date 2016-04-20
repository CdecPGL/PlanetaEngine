#include "TaskSystemKeyPositions.h"

namespace planeta_engine {
	namespace task_sys_key_pos {
		const char* GameObjectUpdatetProcessKeyPositionID("sys_gou"); //�Q�[���I�u�W�F�N�g�X�V�v���Z�X�D��x
		const char* TransformApplyVelocityProcessKeyPositionID("sys_tav"); //�g�����X�t�H�[�����x�K�p�v���Z�X�D��x
		//const int TransformConvertLocalToGlobalProcessPriority(200); //�g�����X�t�H�[�����[�J���O���[�o���ϊ��v���Z�X�D��x
		const char* CollisionDetectProcessKeyPositionID("sys_cd"); //�Փ˔���v���Z�X�D��x
		//const int TransformConvertGlobalToLocalProcessPriority(400);
		const char* GameObjectDrawProcessKeyPositionID("sys_god"); //�Q�[���I�u�W�F�N�g�`��v���Z�X�D��x
		const char* GUIUpdateProcessKeyPositionID("sys_gu"); //UI�X�V�v���Z�X�D��x
		const char* GUIDrawProcessKeyPositionID("sys_gd"); //UI�`��v���Z�X�D��x
	}
}