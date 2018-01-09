# 依存ライブラリ

## vcpkgを用いてインストール可能

- boost 1.66
- cryptopp 5.6.5
- zlib 1.2.11
- lua 5.3.4
- picojson 1.3.0
- TinyXML2 6.0.0

## Portファイルを設定することでvcpkgでインストール可能

- DXライブラリ 3.19
- EffekseerForDXLib 1.22
- LuaIntf 2017.01.06

## EffekseerForDxLib1.3の導入についての問題

APIの変更による問題があるので、いまはまだバージョン1.2を使う。
1.3の対応コードはテスト的に実装したが未検証。
EffekseerUtil.h内に定義されたEFFEKSEER_FOR_DXLIB_VERSIONマクロで切替可能。

### APIの変更への対応

Effekseer3DRendererからGetDeviceメンバ関数がなくなった。
Effekseer3DRendererの子クラスであるEffekseer3DRendererDX9に移動しただけなので、ダウンキャストすることで対処可能。

Effekseer::TextureLoaderのLoad、Unload関数の仕様変更。テクスチャがvoidポインタではなく、Effekseer::TextureData型のポインタで取引されるようになった。
テストをしていないが、一応対応コードを書いた。たぶんちゃんと動かない。
