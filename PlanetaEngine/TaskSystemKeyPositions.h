#pragma once

namespace planeta_engine {
	namespace task_sys_key_pos {
		extern const char* CollisionDetectTaskKeyPositionID; //衝突判定プロセスキーポジションID
		extern const char* GameObjectDrawTaskKeyPositionID; //ゲームオブジェクト描画プロセスキーポジションID
		extern const char* GameObjectUpdatetTaskKeyPositionID; //ゲームオブジェクト更新プロセスキーポジションID
		extern const char* GUIDrawTaskKeyPositionID; //UI描画プロセスキーポジションID
		extern const char* GUIUpdateTaskKeyPositionID; //UI更新プロセスキーポジションID
		extern const char* TransformApplyVelocityTaskKeyPositionID; //トランスフォーム速度適用プロセスキーポジションID
		//extern const int TransformConvertLocalToGlobalProcessPriority; //トランスフォームローカルグローバル変換プロセス優先度
		//extern const int TransformConvertGlobalToLocalProcessPriority; //トランスフォームグローバルローカル変換プロセス優先度
	}
}