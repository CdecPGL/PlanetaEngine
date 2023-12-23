# PlanetaEngine

C++用のコンポーネントベースなゲームエンジン。

未完成です。

## コンパイラ

Visual C++ 製品バージョン17.8.3以上

## 依存ライブラリ

`vcpkg.json`参照。

## 開発者用初期設定

### vcpkgのインストール

vspkgを配置したいディレクトリで以下を実行。（[参考](https://learn.microsoft.com/ja-jp/vcpkg/get_started/get-started-msbuild?pivots=shell-powershell#1---set-up-vcpkg)）

```ps1
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg; .\bootstrap-vcpkg.bat
.\vcpkg.exe integrate install
```

以下の環境変数を設定。

- VCPKG_ROOT: vcpkgを配置したディレクトリを設定
- PATH: vcpkgを配置したディレクトリを追加（既存のものは消さないこと）

### 依存ライブラリのインストール

Visual Studioでビルドすることで自動的にインストールされる。

## ライセンス

[MITライセンス](LICENSE)の下公開しています。
