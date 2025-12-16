# ZMK Auto Caps Word

This module enables automatic activation of ZMK's built-in Caps Word feature.
When two uppercase letters are typed in succession (within a configurable time
window), the module taps your existing `&caps_word` behavior so subsequent
letters are capitalized without holding Shift.

## Usage

1. Add this repository as a ZMK module in your `west.yml`:

   ```yaml
   - name: behavior-auto-capsword
     url: https://example.com/zmk-behavior-auto-capsword.git
   ```

2. Enable Caps Word itself (required dependency):

   ```conf
   CONFIG_ZMK_CAPS_WORD=y
   ```

3. Declare a `caps_word` behavior instance in your keymap (only one is
   supported). Most keymaps already have this, but add it if missing:

   ```dts
   / {
       behaviors {
           caps_word: caps_word {
               compatible = "zmk,behavior-caps-word";
               #binding-cells = <0>;
               label = "CAPS_WORD";
           };
       };
   };
   ```

4. Enable the auto-trigger module and optionally tune its timing in your board
   or shield `.conf`:

   ```conf
   CONFIG_ZMK_BEHAVIOR_AUTO_CAPSWORD=y
   CONFIG_ZMK_BEHAVIOR_AUTO_CAPSWORD_WINDOW_MS=1500  # time between presses
   CONFIG_ZMK_BEHAVIOR_AUTO_CAPSWORD_THRESHOLD=2     # presses required
   ```

5. Build and flash as usual; you do **not** need to bind this behavior to any
   key. The listener is global and will tap your `&caps_word` instance whenever
   two uppercase letters are pressed within the configured window while Shift
   is held.

Once built, the firmware listens for keycode events. If two uppercase keypresses
occur within the configured window while Shift is held, Caps Word is activated
automatically by invoking the `&caps_word` binding.

## Notes

- The detector resets if more time than the configured window passes between
  uppercase inputs.
- Only alphabetic keypresses (A-Z) with an active Shift modifier are counted.
- Logging can be tuned via `CONFIG_ZMK_BEHAVIOR_AUTO_CAPSWORD_LOG_LEVEL`.
