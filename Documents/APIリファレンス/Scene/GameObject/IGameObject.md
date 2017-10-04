# 概要

ゲームオブジェクトへのアクセスを提供するインターフェイス

# 公開関数

## bool Activate()

ゲームオブジェクトをシーン内で有効化する。戻り値は有効化の成否を示す。  

## bool Inativate()

ゲームオブジェクトをシーン内で無効化する。戻り値は無効化の成否を示す。  

## std::shared_ptr\<IGameObject> GetShared_pointer()

ゲームオブジェクトのSharedPointerを取得する。

## template\<typename GOI> std::shared_pr<GOI> GetInterface()

テンプレート引数で指定した型か、その子クラスのインターフェイスを持っていれば返す。持っていない場合はnullptrを返す。

## template\<typename GOI> bool HasInterface()

テンプレート引数で指定した型か、その子クラスのインターフェイスを持っているかどうかを返す。

## template\<class T> util::WeakPointer<T> CreateAndAttachTask(TaskSlot slot)

テンプレート引数で指定した型のタスクを作成し、アタッチする。  
ゲームオブジェクトのアタッチされたタスクの寿命はそのゲームオブジェクトと同じになり、ゲームオブジェクトの無効化有効化に合わせて停止、再開する。

## 