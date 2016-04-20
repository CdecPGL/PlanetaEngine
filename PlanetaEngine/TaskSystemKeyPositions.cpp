#include "TaskSystemKeyPositions.h"

namespace planeta_engine {
	namespace task_sys_key_pos {
		const char* GameObjectUpdatetProcessKeyPositionID("sys_gou"); //ゲームオブジェクト更新プロセス優先度
		const char* TransformApplyVelocityProcessKeyPositionID("sys_tav"); //トランスフォーム速度適用プロセス優先度
		//const int TransformConvertLocalToGlobalProcessPriority(200); //トランスフォームローカルグローバル変換プロセス優先度
		const char* CollisionDetectProcessKeyPositionID("sys_cd"); //衝突判定プロセス優先度
		//const int TransformConvertGlobalToLocalProcessPriority(400);
		const char* GameObjectDrawProcessKeyPositionID("sys_god"); //ゲームオブジェクト描画プロセス優先度
		const char* GUIUpdateProcessKeyPositionID("sys_gu"); //UI更新プロセス優先度
		const char* GUIDrawProcessKeyPositionID("sys_gd"); //UI描画プロセス優先度
	}
}