# Sample App for M5Stacks

M5Stack Core シリーズのサンプルアプリです。

## Usage

platformio.ini の default_envs を切り替える。

```ini
[platformio]
; Core Basic + default
default_envs = core-basic

; Core Basic + BLE 動作確認用
; default_envs = core-basic-ble
```

## Develop

開発する前に `platformio.ini` を確認する。

### 機種の設定

機種ごとの設定する。

```ini
[env]
; 共通の設定

; Core Basic + default
[env:core-basic]
board = m5stack-core-esp32
build_src_filter = 
    +<default/**/*.cpp>
    +<default/**/*.c>
    +<default/**/*.h>

; Core2 + default
[env:core2]
board = m5stack-core2
build_src_filter = 
    +<default/**/*.cpp>
    +<default/**/*.c>
    +<default/**/*.h>
```

### 開発対象の設定

開発対象ごとに、ソースコードのディレクトリ `build_src_filter` やライブラリ `lib_deps` を設定する。


```ini
; Core Basic + BLE 動作確認用
[env:core-basic-ble]
extends = env:core-basic
build_src_filter = 
    +<ble/**/*.cpp>
    +<ble/**/*.c>
    +<ble/**/*.h>

; Core Basic + 温湿度気圧センサーユニット Ver.4
[env:core-basic-env4]
extends = env:core-basic
lib_deps =
    ${env:core-basic.lib_deps}
    m5stack/M5Unit-ENV@1.2.0
build_src_filter = 
    +<env4/**/*.cpp>
    +<env4/**/*.c>
    +<env4/**/*.h>
```

### 自作ライブラリ

自作ライブラリは lib/ で開発するのが望ましい。lib/ に保存したライブラリは、すべての env に影響する。特定 env のみで利用するライブラリを使う場合は、lib/ ではなく 特定 env に対応する src/ 配下のディスプレイで開発してください。
