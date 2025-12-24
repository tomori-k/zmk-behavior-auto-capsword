# zmk-behavior-auto-capsword

ZMK 用の Auto Caps Word behavior モジュール。

## 使い方

1. west manifest にこのリポジトリを追加する
2. behavior に次のノードを定義する

    ```dts
    / {
        behaviors {
            auto_caps_word: auto_caps_word {
                compatible = "zmk,behavior-auto-caps-word";
                #binding-cells = <0>;
                continue-list = <UNDERSCORE BACKSPACE DELETE>;
                display-name = "Auto Caps Word";
                auto-activate-upper-count = <2>;
            };
        };
    };
    ```

    - 日本語配列の場合は `UNDERSCORE` の代わりに `INT1` を continue-list に設定する
