# 描画について

## 描画処理

描画はScreenDrawerを通して、Screenに対して行う。
ScreenDrawerはScreenに描画予約を行い、ScreenはDrawManagerからの呼び出しで予約された描画をまとめて行う。

## カメラ

DrawManagerによる一斉描画はシーン全ての更新が終わったあとに行われるので、上記描画の仕組みにより、いつ描画予約を行ったかによらずカメラの姿勢はシーン更新後のものが適用されることになる。カメラ姿勢(位置や角度)の変更は即座に反映され、スクリーンゲームオブジェクト間座標変換についても同じである。

例えば

1. Aの描画予約
1. カメラ姿勢更新
1. Bの描画予約

の順にシーン内で行った場合、Aの描画にもBの描画にもシーン更新終了時点でのカメラ姿勢が適用される。

## 惑星の描画方法

惑星を水平、垂直方向に分割する。(hs,vs)
惑星の外側から、座標系の回転正方向にインデックスを割り振っていく。中心はインデックス0とする。
こうすると、頂点数hs*vs+1、ポリゴン数hs*(vs-1)*2+hsとなる。
