#include QMK_KEYBOARD_H
#include "quantum.h"
#include "print.h"
#include "wls/wls.h"
#include "wireless.h"
#include "usb_main.h"
#include "lowpower.h"
#include "uart.h"

enum __layers { WIN_B, WIN_FN };

typedef union {
    uint32_t raw;
    struct {
        uint8_t flag : 1;
        uint8_t devs : 3;
        uint8_t chk_bat : 1;
        uint8_t record_last_mode;
        uint8_t last_btdevs : 3;
    };
} confinfo_t;
confinfo_t confinfo;

uint32_t post_init_timer = 0x00;
bool     lower_sleep     = false;

void eeconfig_confinfo_init(void) {
    confinfo.raw = eeconfig_read_kb();
    if (!confinfo.raw) {
        confinfo.flag             = true;
        confinfo.chk_bat          = false;
        confinfo.record_last_mode = 0xff;
        confinfo.last_btdevs      = 1;
        confinfo.devs             = DEVS_USB;

        eeconfig_init_kb();
        eeconfig_update_kb(confinfo.raw);
    }
}

void usb_power_connect(void) {
#ifdef USB_POWER_EN_PIN
    gpio_write_pin_low(USB_POWER_EN_PIN);
#endif
}

void usb_power_disconnect(void) {
#ifdef USB_POWER_EN_PIN
    gpio_write_pin_high(USB_POWER_EN_PIN);
#endif
}

void suspend_power_down_kb(void) {
#ifdef LED_POWER_EN_PIN
    gpio_write_pin_low(LED_POWER_EN_PIN);
#endif

    suspend_power_down_user();
}

void suspend_wakeup_init_kb(void) {
#ifdef LED_POWER_EN_PIN
    gpio_write_pin_high(LED_POWER_EN_PIN);
#endif

    int current = wireless_get_current_devs();
    wireless_devs_change(current, current, false);
    suspend_wakeup_init_user();
    hs_rgb_blink_set_timer(timer_read32());
}

bool lpwr_is_allow_timeout_hook(void) {
    // block lowpower mode if we're set to usb
    if (wireless_get_current_devs() == DEVS_USB) {
        return false;
    }

    return true;
}

void wireless_devs_change_kb(uint8_t old_devs, uint8_t new_devs, bool reset) {
    int current = wireless_get_current_devs();
    if (confinfo.devs != current) {
        confinfo.devs = current;
        if (confinfo.devs > 0 && confinfo.devs < 4) confinfo.last_btdevs = confinfo.devs;
        eeconfig_update_kb(confinfo.raw);
    }
}

void wireless_post_task(void) {
    // auto switching devs
    if (post_init_timer && timer_elapsed32(post_init_timer) >= 100) {
        md_send_devctrl(MD_SND_CMD_DEVCTRL_FW_VERSION);   // get the module fw version.
        md_send_devctrl(MD_SND_CMD_DEVCTRL_SLEEP_BT_EN);  // timeout 30min to sleep in bt mode, enable
        md_send_devctrl(MD_SND_CMD_DEVCTRL_SLEEP_2G4_EN); // timeout 30min to sleep in 2.4g mode, enable
        wireless_devs_change(!confinfo.devs, confinfo.devs, false);
        post_init_timer = 0x00;
    }

    // hs_mode_scan(false, confinfo.devs, confinfo.last_btdevs);
}

void lpwr_wakeup_hook(void) {
    // hs_mode_scan(false, confinfo.devs, confinfo.last_btdevs);

    // gpio_write_pin_high(LED_POWER_EN_PIN);
    // gpio_write_pin_high(A9);
    // gpio_write_pin_high(HS_LED_BOOSTING_PIN);
}

uint32_t wls_process_long_press(uint32_t trigger_time, void *cb_arg) {
    uint16_t keycode = *((uint16_t *)cb_arg);

    switch (keycode) {
        case BT_PRF1: {
            uint8_t mode = confinfo.devs;
            hs_modeio_detection(true, &mode, confinfo.last_btdevs);
            if ((mode == hs_bt) || (mode == hs_wireless) || (mode == hs_none)) {
                wireless_devs_change(wireless_get_current_devs(), DEVS_BT1, true);
            }
        } break;
        case BT_PRF2: {
            uint8_t mode = confinfo.devs;
            hs_modeio_detection(true, &mode, confinfo.last_btdevs);
            if ((mode == hs_bt) || (mode == hs_wireless) || (mode == hs_none)) {
                wireless_devs_change(wireless_get_current_devs(), DEVS_BT2, true);
            }
        } break;
        case BT_PRF3: {
            uint8_t mode = confinfo.devs;
            hs_modeio_detection(true, &mode, confinfo.last_btdevs);
            if ((mode == hs_bt) || (mode == hs_wireless) || (mode == hs_none)) {
                wireless_devs_change(wireless_get_current_devs(), DEVS_BT3, true);
            }
        } break;
        case OU_2P4G: {
            uint8_t mode = confinfo.devs;
            hs_modeio_detection(true, &mode, confinfo.last_btdevs);
            if ((mode == hs_2g4) || (mode == hs_wireless) || (mode == hs_none)) {
                wireless_devs_change(wireless_get_current_devs(), DEVS_2G4, true);
            }
        } break;
        case EE_CLR: {
        } break;
        default:
            break;
    }

    return 0;
}

bool process_record_wls(uint16_t keycode, keyrecord_t *record) {
    static uint16_t       keycode_shadow               = 0x00;
    static deferred_token wls_process_long_press_token = INVALID_DEFERRED_TOKEN;

    keycode_shadow = keycode;

#ifndef WLS_KEYCODE_PAIR_TIME
#    define WLS_KEYCODE_PAIR_TIME 3000
#endif

#define WLS_KEYCODE_EXEC(wls_dev)                                                                                          \
    do {                                                                                                                   \
        if (record->event.pressed) {                                                                                       \
            if (wireless_get_current_devs() != wls_dev) wireless_devs_change(wireless_get_current_devs(), wls_dev, false); \
            if (wls_process_long_press_token == INVALID_DEFERRED_TOKEN) {                                                  \
                wls_process_long_press_token = defer_exec(WLS_KEYCODE_PAIR_TIME, wls_process_long_press, &keycode_shadow); \
            }                                                                                                              \
        } else {                                                                                                           \
            cancel_deferred_exec(wls_process_long_press_token);                                                            \
            wls_process_long_press_token = INVALID_DEFERRED_TOKEN;                                                         \
        }                                                                                                                  \
    } while (false)

    switch (keycode) {
        case BT_PRF1: {
            uint8_t mode = confinfo.devs;
            hs_modeio_detection(true, &mode, confinfo.last_btdevs);
            if ((mode == hs_bt) || (mode == hs_wireless) || (mode == hs_none)) {
                WLS_KEYCODE_EXEC(DEVS_BT1);
                // hs_rgb_blink_set_timer(timer_read32());
            }

        } break;
        case BT_PRF2: {
            uint8_t mode = confinfo.devs;
            hs_modeio_detection(true, &mode, confinfo.last_btdevs);
            if ((mode == hs_bt) || (mode == hs_wireless) || (mode == hs_none)) {
                WLS_KEYCODE_EXEC(DEVS_BT2);
                // hs_rgb_blink_set_timer(timer_read32());
            }
        } break;
        case BT_PRF3: {
            uint8_t mode = confinfo.devs;
            hs_modeio_detection(true, &mode, confinfo.last_btdevs);
            if ((mode == hs_bt) || (mode == hs_wireless) || (mode == hs_none)) {
                WLS_KEYCODE_EXEC(DEVS_BT3);
                // hs_rgb_blink_set_timer(timer_read32());
            }
        } break;
        case OU_2P4G: {
            uint8_t mode = confinfo.devs;
            hs_modeio_detection(true, &mode, confinfo.last_btdevs);
            if ((mode == hs_2g4) || (mode == hs_wireless) || (mode == hs_none)) {
                WLS_KEYCODE_EXEC(DEVS_2G4);
                // hs_rgb_blink_set_timer(timer_read32());
            }
        } break;
        case OU_USB: {
            wireless_devs_change(wireless_get_current_devs(), DEVS_USB, false);
        } break;
        default:
            return true;
    }

    return false;
}

static SerialConfig serialConfig = {
    SERIAL_DEFAULT_BITRATE, UART_WRDLEN, UART_STPBIT, UART_PARITY, UART_ATFLCT,
};

void keyboard_post_init_kb(void) {
    // initialize configuration from eeprom
    eeconfig_confinfo_init();

    // initialize wireless devices and reconnect to last setting
    wireless_init();
    wireless_devs_change(!confinfo.devs, confinfo.devs, false);

    // Enable open drain pin on mcu for LED-V power circuit
    gpio_set_pin_output_open_drain(B7);
    gpio_write_pin_low(B7);

    post_init_timer = timer_read32();
    wait_ms(50);
    keyboard_post_init_user();
}

bool rgb_matrix_indicators_kb(void) {
    if (!rgb_matrix_indicators_user()) {
        return false;
    }
    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(42, 255, 255, 255);
    } else {
        rgb_matrix_set_color(42, 0, 0, 0);
    }
    return true;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) {
        return false;
    }
    if (!process_record_wls(keycode, record)) {
        return false;
    }
    switch (keycode) {
        case DF(WIN_B):
            if (record->event.pressed) {
                set_single_persistent_default_layer(WIN_B);
                layer_state_set(1 << WIN_B);
            }
            return false;

        default:
            return true;
    }
}
