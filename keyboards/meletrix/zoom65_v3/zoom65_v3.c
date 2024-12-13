/* Copyright (C) 2023 jonylee@hfd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "quantum.h"
#include "print.h"

enum __layers { WIN_B, WIN_FN };

void keyboard_post_init_user(void) {
    // Enable open drain pin on mcu for LED-V power circuit
    gpio_set_pin_output_open_drain(B7);
}

bool rgb_matrix_indicators_kb(void) {
    if (!rgb_matrix_indicators_user()) {
        return false;
    }
    if (host_keyboard_led_state().caps_lock) {
        // RGB_MATRIX_INDICATOR_SET_COLOR(42, 255, 255, 255);
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
