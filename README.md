# zmk-behavior-auto-capsword

ZMK 用の Auto Caps Word behavior モジュール。

## 使い方

1. west manifest にこのリポジトリを追加する
2. keymap か overlay で `#include <behaviors/auto_caps_word.dtsi>` を追加する
3. keymap で `&auto_caps_word` を使う

## 提供ファイル

- `src/behavior_auto_caps_word.c`
- `dts/bindings/behaviors/zmk,behavior-auto-caps-word.yaml`
- `dts/behaviors/auto_caps_word.dtsi`
