#include QMK_KEYBOARD_H

enum __layers { WIN_B, WIN_FN };

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_B] = LAYOUT( /* Base */
        QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,    KC_EQL,  KC_BSPC, _______, _______,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,    KC_RBRC, KC_BSLS, KC_PGUP,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,             KC_ENT,  KC_PGDN,
        KC_LSFT, _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,    KC_RSFT, KC_UP,   KC_END,
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                                      KC_RALT, MO(WIN_FN), KC_LEFT, KC_DOWN, KC_RGHT),

    [WIN_FN] = LAYOUT( /* Fn */
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  _______, _______,
        _______, BT_PRF1, BT_PRF2, BT_PRF3, OU_2P4G, OU_USB , _______, _______, KC_INS,  _______, KC_PSCR, _______, SC_TOGG, UG_NEXT, SC_UP,
        _______, BT_PRF4, BT_PRF5, _______, _______, _______, _______, _______, _______, UG_TOGG, _______, _______,          UG_HUEU, SC_DOWN,
        _______, _______, _______, _______, EE_CLR,  _______, QK_BOOT, _______, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, UG_VALU, SC_SWCH,
        _______, GU_TOGG, _______,                   _______,                                     _______, _______, UG_SATD, UG_VALD, UG_SATU),

};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [WIN_B] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [WIN_FN] = { ENCODER_CCW_CW(UG_HUED, UG_HUEU) },

};
#endif
