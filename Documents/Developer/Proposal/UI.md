# UIについて

## 各要素の説明

### UIComponent

ボックス、ボタン、パネルなどUIの部品を表すクラス。Compositパターンをもとに設計。

### UIObject

UIComponetを管理しあるひとまとまりの機能を提供するクラス。Mediatorパターンのような役割をもつ。ゲームプロセスなどは基本的にUIObjectと介してUIを扱う。

## 位置について

### UIObject

各々が画面座標を持つ。

### UIComponent

親を基準とする相対座標をもつ。最終的なサイズ位置は、親の座標サイズ、相対座標サイズ、辺固定情報から求められる。
最終位置サイズの決定の優先順位は
親の座標サイズ>辺固定情報>相対座標サイズ
最上位のUIComponentの親はUIObjectとなる。
