# 衝突判定について

現状では、コライダーをすべて同じように扱っているが、通常のコライダーと地形コライダーを分けるのはどうか。
地形コライダー同氏は衝突しない。通常コライダー同氏はする。
通常コライダーは、それを所有するゲームオブジェクトが所属する地形のもつコライダーにのみ衝突する。

各コライダーは衝突イベント関数を持つ。
所属する地形との衝突イベントと、それ以外の衝突コンポーネントとの衝突イベント。
地形衝突イベントについては、引数から衝突した直後か、衝突中か、衝突が終わった直後かを判定できる。
今後、所属地形以外の地形とは、通常コライダーと同じ扱うの衝突判定をさせる予定。

## 2015/9/13

地形コライダーと通常コライダーは分けた。
GroundComponentが地形コライダーの役割も持つ。
GroundComponentは各通常コライダーとの衝突判定を実装することができる。
通常コライダーは、地形衝突フラグを持ち、フラグが立っている場合そのげーむおぶじぇくとの所属する地形と衝突する。

## 2016/1/29

地形座標再導入に伴い、パフォーマンスの改善を見込んで地形の衝突判定を地形座標を用いたものに変更しようとしたが問題があるのでグローバル座標を用いたもののままにした。
コライダーはそれぞれゲームオブジェクトからの相対位置を持っているが、それを加味した地形座標を求めなければならない。
相対座標をグラウンドコンポーネントのベクトル正規化関数で正規化して求めゲームオブジェクトの地形座標に加算する方法は、計算コストがかからないが、グローバル座標を用いて計算している描画コンポーネントとのズレが生じてしまう。
相対座標とゲームオブジェクトのグローバル座標を加算してから地形座標に変換する方法では描画位置とのズレは生じないが、計算コストが単純にグローバル座標を用いて計算する場合と同じになってしまう。
これ以外に方法を思いつかなかったので、グローバル座標を用いる従来の方法を引き続き利用することにした。