﻿# ゲームのディレクトリについて

## ディレクトリ構造

[Root]
|[resources](ゲームリソースを格納)
||[dev_data](開発用データフォルダ。開発時にのみ有効化)
||data(リリース用リソースアーカイブファイル)
|[system](システムファイルを格納)
||system_config(システム設定)
|[config](ユーザー設定ファイルを格納)
||user_config.json(ユーザー設定)
||key_config.ini(キーコンフィグ)
|[log](ログを格納)
||Log.txt(DXライブラリのログ)
||pe_log.txt(PlanetaEngineのログ)
|[save_datas](セーブデータを格納)
||
|game.exe(ゲーム本体)
