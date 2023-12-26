# PlanetaEngine

C++用のコンポーネントベースなゲームエンジン。

未完成です。

## コンパイラ

Visual C++ 製品バージョン17.8.3以上

## 利用者向け初期設定（作成中）

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

### vcpkgの設定

`vcpkg-configuration.json`の`registries`に以下を追加する。

```json
    {
      "kind": "git",
      "repository": "https://github.com/CdecPGL/vcpkg-registory-for-game-dev",
      "baseline": "e9c268ad4daaa7b77825b755ddfdd98df95f7a24",
      "packages": [ "planeta-engine", "dxlib", "effekseer-for-dxlib", "lua-intf" ]
    }
```

以下のコマンドを実行する。

```ps1
vcpkg add port planeta-engine
```

### Visual Studioプロジェクトの作成と設定

1. [DXライブラリ向け設定](https://dxlib.xsrv.jp/use/dxuse_vscom2022.html)を行う
1. [vcpkg向け設定](https://learn.microsoft.com/ja-jp/vcpkg/get_started/get-started-msbuild?pivots=shell-powershell#3---set-up-the-project-files)を行う
1. プロジェクト設定において以下の設定を行う
    1. 「構成プロパティ」->「vcpkg」->「Use Static Libraries」を「はい」にする
    1. 「構成プロパティ」->「C/C++」->「言語」->「C++言語標準」を「ISO C++20標準」にする
    1. 「構成プロパティ」->「C/C++」->「言語」->「C++言語標準」で以下を「はい」にする
        - forループスコープの強制準拠
        - 型変換規則を強制する
        - 準拠モード
    1. 「構成プロパティ」->「リンカー」->「コマンドライン」->「追加のオプション」に「/ignore:4099」を追加する
        - EffekseerForDxLibのリンク時に4099警告が出るが実害はないため

### 依存ライブラリのインストール

Visual Studioでビルドすることで自動的にインストールされる。

## 開発者用初期設定

### vcpkgのインストール

利用者向け初期設定と同様。

### 依存ライブラリのインストール

Visual Studioでビルドすることで自動的にインストールされる。

## ライセンス

[MITライセンス](LICENSE)の下公開しています。
