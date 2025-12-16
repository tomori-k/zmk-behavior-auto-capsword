#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <stdbool.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zephyr/usb/hid.h>

#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/hid.h>

#include <auto_capsword.h>

LOG_MODULE_REGISTER(auto_capsword, CONFIG_ZMK_BEHAVIOR_AUTO_CAPSWORD_LOG_LEVEL);

BUILD_ASSERT(DT_HAS_COMPAT_STATUS_OKAY(zmk_behavior_caps_word),
             "Auto caps word requires a caps_word behavior instance");

static const struct device *const caps_word_dev = DEVICE_DT_GET_ONE(zmk_behavior_caps_word);

static uint8_t consecutive_uppercase;
static int64_t last_uppercase_ts;

static bool is_letter_usage(uint32_t usage_page, uint32_t usage_id) {
    return usage_page == HID_USAGE_KEY &&
           usage_id >= HID_USAGE_KEY_KEYBOARD_A && usage_id <= HID_USAGE_KEY_KEYBOARD_Z;
}

static bool has_shift_mods(const struct zmk_keycode_state_changed *ev) {
    uint8_t mods = ev->explicit_modifiers | ev->implicit_modifiers;
    return (mods & (MOD_LSFT | MOD_RSFT)) != 0;
}

static bool is_uppercase_press(const struct zmk_keycode_state_changed *ev) {
    if (!ev->state) {
        return false;
    }

    const uint32_t usage_page = HID_USAGE_PAGE(ev->keycode);
    const uint32_t usage_id = HID_USAGE_ID(ev->keycode);

    if (!is_letter_usage(usage_page, usage_id)) {
        return false;
    }

    return has_shift_mods(ev);
}

static void tap_caps_word(const struct zmk_keycode_state_changed *src_ev) {
    if (!device_is_ready(caps_word_dev)) {
        LOG_WRN("Caps Word behavior not ready");
        return;
    }

    const struct zmk_behavior_binding binding = {
        .behavior_dev = caps_word_dev,
        .param1 = 0,
        .param2 = 0,
    };

    const struct zmk_behavior_binding_event bev = {
        .position = 0,
        .layer = 0,
        .timestamp = src_ev->timestamp,
    };

    zmk_behavior_invoke_binding(&binding, bev, true);
    zmk_behavior_invoke_binding(&binding, bev, false);

    consecutive_uppercase = 0;
    last_uppercase_ts = 0;
}

void auto_capsword_reset(void) {
    consecutive_uppercase = 0;
    last_uppercase_ts = 0;
}

static int auto_capsword_listener(const struct zmk_event_header *eh) {
    const struct zmk_keycode_state_changed *ev =
        as_zmk_keycode_state_changed(eh);

    if (!ev || !CONFIG_ZMK_BEHAVIOR_AUTO_CAPSWORD) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    if (!is_uppercase_press(ev)) {
        auto_capsword_reset();
        return ZMK_EV_EVENT_BUBBLE;
    }

    const int64_t now = ev->timestamp;

    if (last_uppercase_ts > 0 &&
        (now - last_uppercase_ts) > CONFIG_ZMK_BEHAVIOR_AUTO_CAPSWORD_WINDOW_MS) {
        consecutive_uppercase = 0;
    }

    consecutive_uppercase++;
    last_uppercase_ts = now;

    if (consecutive_uppercase >= CONFIG_ZMK_BEHAVIOR_AUTO_CAPSWORD_THRESHOLD) {
        LOG_DBG("Auto caps word triggered after %u uppercase characters", consecutive_uppercase);
        tap_caps_word(ev);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(auto_capsword, auto_capsword_listener);
ZMK_SUBSCRIPTION(auto_capsword, zmk_keycode_state_changed);
