# コンパイルオプション

## VC++15.5

### 命令セット

/arch:sse2。
AVXにするとCoreix-2***以上になってしまうため。

### 浮動小数点モデル

/fp:fast。
floatの計算で精度を再現するために計算が遅くなることがなくなり、doubleとの速度差はなくなる。ただし移植性は落ちるので、Androidなどに導入した際に精度による問題が起こったら変更を考える必要がある。

### 浮動小数点例外

無効

### C++言語標準

/std:c++17

### エラー抑制

#### C++17対応

C++17発生するBoostLibraryとCrypto++のエラー抑制。
定義済みマクロに以下を追加

- _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
- _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
- _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING

#### VC15.5

VC15.5で発生するBoostLibraryの警告抑制。
定義済みマクロに以下を追加

- BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#### boost::signals2関連で発生するc4996エラー

 プロジェクト設定->C/C++->コマンドラインに"-D_SCL_SECURE_NO_WARNINGS"を追加
