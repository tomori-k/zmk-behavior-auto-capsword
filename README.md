# ZMK Auto Caps Word

This module enables automatic activation of ZMK's built-in Caps Word feature.
When two uppercase letters are typed in succession (within a configurable time
window), the module taps your existing `&caps_word` behavior so subsequent
letters are capitalized without holding Shift.

## Usage

1. Add this repository as a ZMK module in your `west.yml`.
2. Ensure `CONFIG_ZMK_CAPS_WORD=y` is enabled in your shield/board configuration.
3. Ensure your keymap defines a `caps_word` behavior instance. The module will
   automatically tap that behavior when the trigger is met (only one instance
   is supported).
4. Enable the module (on by default) and adjust the detection window if desired:

```conf
CONFIG_ZMK_BEHAVIOR_AUTO_CAPSWORD=y
CONFIG_ZMK_BEHAVIOR_AUTO_CAPSWORD_WINDOW_MS=1500
CONFIG_ZMK_BEHAVIOR_AUTO_CAPSWORD_THRESHOLD=2
```

Once built, the firmware listens for keycode events. If two uppercase keypresses
occur within the configured window while Shift is held, Caps Word is activated
automatically by invoking the `&caps_word` binding.

## Notes

- The detector resets if more time than the configured window passes between
  uppercase inputs.
- Only alphabetic keypresses (A-Z) with an active Shift modifier are counted.
- Logging can be tuned via `CONFIG_ZMK_BEHAVIOR_AUTO_CAPSWORD_LOG_LEVEL`.
