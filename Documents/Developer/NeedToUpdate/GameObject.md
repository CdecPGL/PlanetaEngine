# GameObject

## 生成、破棄

生成は専用のファクトリ関数を通して行い、常にスマポでラップする。

上記手法以外での生成は禁止だが、new,delete演算子、コンストラクタをprivate指定するとスマートポインタの作成が面倒くさくなるので、現状では特に対策は行わない。
コピーは不可能にする。

## 管理

Sceneの持つGameObjectManagerに登録することがシーンに登録することとなる。
管理はGameObjectManagerが行い、有効無効に関わらずGameObjectManagerが実態を保持する。
GameObjectManagerの管理外のGameObjectは許されない。将来的にシーン間での引継ぎを可能にする予定。

登録時にIDが設定される。GameObjectは名前を付けて登録することができ、その場合は名前で検索することができる。外部で扱う場合は、WeakPointerを通して行う。

## コンポーネントの初期化タイミング

コンポーネントは、GameObjectの初期化前に追加し、GameObjectの初期化時に初期化される。

## コンポーネントの更新

コンポーネントには更新関数はない。タスクを用いて更新を行う。

## 親子関係について

親子関係はゲームオブジェクトが管理することにしていたが、その役割をトランスフォームコンポーネントに移動することにした。その方が親子関係による位置の依存の処理が行いやすい。

同じ理由で地形所属の管理もトランスフォームコンポーネントが行う。

## ゲームオブジェクトのライフサイクル(2016/7/18更新。その後に初期化フローの変更を行ったので、この項目も修正が必要)

### インスタンス化[GameObjectTemplateHolderが担当]

該当するゲームオブジェクトの初回作成時にのみ実行される。この処理を経て作成されたゲームオブジェクトはテンプレートとして保持される。
テンプレートとして保持されたクラスは、初期化以降の処理は行われない。

#### コンストラクタ

#### GameObjectBase::SetSceneData

#### AddAndSetUpComponents

コンポーネントの追加と設定を行う

##### GameObjectBase::AddCOmponentsFromTypeIDList_

- コンポーネントの追加。GOCTypeIDリストが指定された場合はそのリストが、Ptreeが指定された場合はそこから作成されたリストが渡される。エイリアスの設定もこの関数が行う。

##### GameObjectBase::SetSceneAndGODataToComponents

- GameObjectComponent::SetSceneAndHolderGOData

##### GameObjectBase::SetDataToComponentsFromPtree_(Ptreeが指定された場合)

- 各コンポーネントへPtreeからデータをセット

### 複製[GameObjectTemplateHolderが担当]

該当するゲームオブジェクトの作成時、テンプレートからのクローンにおいて実行される

#### リフレクションシステムによりゲームオブジェクトを作成

#### GameObjectBase::ProcessClonation

- シーンデータのセット
- リフレクションシステムによるコンポーネントのクローン
- GameObjectBase::SetSceneAndGODataToComponents

### 初期化[GameObjectManagerが担当]

該当するゲームオブジェクトが作成されるたびに実行される

#### GameObjectBase::SetManagerConnection

#### GameObjectBase::ProcessInitialization

- GameObjectComponent::GetOtherComponentsProc: 保持されているGOのほかのコンポーネントを取得する。初期化処理は別途OnInitializedに記述。初期化はすべてのコンポーネントがほかのコンポーネントへの参照を取得した後に行うことが望ましいので、OnInitializedとは分けた。
- GameObjectComponent::Initialize: コンポーネントの初期化
  - GameObjectComponent::OnInitialized
- 初期化イベント: プロセスの追加などを行う

### 有効化[GameObjectManagerが担当]

ゲームオブジェクトが有効化される度に実行される

#### GameObjectBase::ProcessActivation

- GameObjectComponent::Activate: コンポーネントの有効化
  - GameObjectComponent::OnActivated
- アタッチされたタスクの再開
- 有効化イベント

### 無効化[GameObjectManagerが担当]

ゲームオブジェクトが無効化される度に実行される

#### GameObjectBase::ProcessInactivation

- 無効化イベント
- アタッチされたタスクの停止
- GameObjectComponent::Inactivate: コンポーネントの無効化
  - GameObjectComponent::OnInactivated

### 破棄[GameObjectManagerが担当]

ゲームオブジェクトの破棄時に実行される

#### GameObjectBase::ProcessDisposal

- 破棄時イベント
- GameObjectComponent::Finalize: コンポーネントの終了処理
  - GameObjectComponent::OnFinalized
- アタッチされたタスクの破棄

## シーン遷移時のゲームオブジェクト引き継ぎについて

夢幻の緑星では実装しない

実装する場合、オブジェクトの整合性が問題になる。
遷移時にシーンマネージャのインスタンスは変わるが、オブジェクトがその参照を持っていた場合、何らかの方法で参照を更新する必要がある。また、システムタスクについても遷移時にインスタンスを変えるなら同様。

## Todo

### 実装すべき機能

- 生成、破棄の隠ぺい
- 複製を簡単に行うためのCloneメンバ関数実装
