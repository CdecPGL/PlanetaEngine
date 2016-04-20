#pragma once

namespace planeta_engine {
	namespace task_sys_key_pos {
		extern const char* CollisionDetectProcessKeyPositionID; //衝突判定プロセスキーポジションID
		extern const char* GameObjectDrawProcessKeyPositionID; //ゲームオブジェクト描画プロセスキーポジションID
		extern const char* GameObjectUpdatetProcessKeyPositionID; //ゲームオブジェクト更新プロセスキーポジションID
		extern const char* GUIDrawProcessKeyPositionID; //UI描画プロセスキーポジションID
		extern const char* GUIUpdateProcessKeyPositionID; //UI更新プロセスキーポジションID
		extern const char* TransformApplyVelocityProcessKeyPositionID; //トランスフォーム速度適用プロセスキーポジションID
		//extern const int TransformConvertLocalToGlobalProcessPriority; //トランスフォームローカルグローバル変換プロセス優先度
		//extern const int TransformConvertGlobalToLocalProcessPriority; //トランスフォームグローバルローカル変換プロセス優先度
	}
}