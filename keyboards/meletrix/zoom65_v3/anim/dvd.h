/* DVD Logo Bouncing Effect */

#define MATRIX_WIDTH 224
#define MATRIX_HEIGHT 64
#define LOGO_R_HEIGHT 12.8
#define LOGO_R_WIDTH 14.0
#define DVD_MIN_FRAMES 1
#define DVD_MAX_FRAMES 5

static int     dvd_tick  = 0;
static float   dvd_x     = 112.0f; // X-coordinate
static float   dvd_y     = 32.0f;  // Y-coordinate
static float   dvd_vx    = 0.4f;   // X velocity
static float   dvd_vy    = 0.3f;   // Y velocity
static uint8_t dvd_color = 100;

static bool DVD_LOGO(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    int tickrate = DVD_MIN_FRAMES + ((255 - rgb_matrix_config.speed) * (DVD_MAX_FRAMES - DVD_MIN_FRAMES)) / 255;
    dvd_tick++;
    if (dvd_tick % tickrate == 0) {
        int v = rgb_matrix_get_hsv().v;

        // Update position
        dvd_x += dvd_vx;
        dvd_y += dvd_vy;

        // Collision detection for boundaries
        if (dvd_x <= LOGO_R_WIDTH * 0.5f || dvd_x >= MATRIX_WIDTH - 1 - LOGO_R_WIDTH * 0.5f) {
            dvd_vx    = -dvd_vx; // Reverse X velocity
            dvd_color = (dvd_color + 16) % 255;
        }
        if (dvd_y <= LOGO_R_HEIGHT * 0.5f || dvd_y >= MATRIX_HEIGHT - 1 - LOGO_R_HEIGHT * 0.5f) {
            dvd_vy    = -dvd_vy; // Reverse Y velocity
            dvd_color = (dvd_color + 16) % 255;
        }

        HSV hsv = rgb_matrix_config.hsv;
        hsv.h   = dvd_color;
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);

        // Render the current position as the "logo"
        for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
            // Calculate LED position as (x, y)
            uint8_t x = g_led_config.point[i].x;
            uint8_t y = g_led_config.point[i].y;

            // Determine if this LED is close to the "logo" position
            if (fabsf(x - dvd_x) < LOGO_R_WIDTH && fabsf(y - dvd_y) < LOGO_R_HEIGHT) {
                // Set the LED color to the current hue
                rgb_matrix_set_color(i, rgb.r * v / 255, rgb.g * v / 255, rgb.b * v / 255);
            } else {
                // Turn off other LEDs
                rgb_matrix_set_color(i, 0, 0, 0);
            }
        }
    }

    return rgb_matrix_check_finished_leds(led_max);
}
