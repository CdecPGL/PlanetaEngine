# タスク

ゲーム内の処理を行うためのシステム。
継承することでカスタマイズされたタスクを定義することが可能。
ゲームオブジェクトの更新は、タスクスロットにタスクを登録することで行う。

## タスクスロット

タスクはタスクスロットに登録され、実行される。
タスクスロットは実行順序を定義したものであり、タスクスロットには任意のタスクを登録することができる。各タスクには名前があり、定数が定義されている。
TASK_SLOTは静的配列で管理され、以下の種類が存在する。

- TASK_SLOT_0[PLAYER_INPUT_UPDATE_PHASE]  
プレイヤー入力処理タスク用スロット
- TASK_SLOT_1[PRE_COLLISON_EARLY_PHASE]  
衝突前判定前の前半にに実行されるスロット
- TASK_SLOT_2[PRE_COLLISON_LATE_PHASE]  
衝突前判定前の後半に実行されるスロット
- (GameObject速度適用)
- (衝突判定)
- TASK_SLOT_3[POST_COLLISON_EARLY_PHASE]  
衝突前判定後の前半にに実行されるスロット
- TASK_SLOT_4[POST_COLLISON_LATE_PHASE]  
衝突前判定後の後半にに実行されるスロット
- TASK_SLOT_5[EVENT_UPDATE_PHASE]  
イベントの更新タスク用のスロット
- TASK_SLOT_6[CAMERA_UPDATE_PHASE]  
カメラ更新タスク用のスロット
- (カメラの状態適用)
- TASK_SLOT_7[POST_CAMERA_UPDATE_PHASE]  
カメラ更新後に実行されるスロット。オブジェクトの向きをカメラに合わせるタスクなどで使用。
- TASK_SLOT_8[GUI_UPDATE_EARLY_PHASE]  
GUIの更新前半スロット
- TASK_SLOT_9[GUI_UPDATE_LATE_PHASE]  
GUIの更新後半スロット
- TASK_SLOT_10[PRE_DRAW_UPDATE_PHASE]  
描画前に実行されるスロット
- (GameObject描画)
- (GUI描画)