/* DVD Logo Bouncing Effect */

// Define boundaries for the field, should match space used for rgb positions
#define MATRIX_WIDTH 224
#define MATRIX_HEIGHT 64
#define LOGO_R_HEIGHT 12.8
#define LOGO_R_WIDTH 14.0

// Global state variables for logo position and velocity
static float   dvd_x        = 112.0f; // X-coordinate
static float   dvd_y        = 32.0f;  // Y-coordinate
static float   dvd_vx       = 0.4f;   // X velocity
static float   dvd_vy       = 0.3f;   // Y velocity
static uint8_t dvd_color[3] = {100, 100, 100};

void increment_color(void) {
    dvd_color[0] += rand() % 32;
    dvd_color[1] += rand() % 32;
    dvd_color[2] += rand() % 32;
}

// Function to render the DVD effect
static bool dvd(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // Update position
    dvd_x += dvd_vx;
    dvd_y += dvd_vy;

    // Collision detection for boundaries
    if (dvd_x <= LOGO_R_WIDTH * 0.5f || dvd_x >= MATRIX_WIDTH - 1 - LOGO_R_WIDTH * 0.5f) {
        dvd_vx = -dvd_vx; // Reverse X velocity
        increment_color();
    }
    if (dvd_y <= LOGO_R_HEIGHT * 0.5f || dvd_y >= MATRIX_HEIGHT - 1 - LOGO_R_HEIGHT * 0.5f) {
        dvd_vy = -dvd_vy; // Reverse Y velocity
        increment_color();
    }

    // Render the current position as the "logo"
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        // Calculate LED position as (x, y)
        uint8_t x = g_led_config.point[i].x;
        uint8_t y = g_led_config.point[i].y;

        // Determine if this LED is close to the "logo" position
        if (fabsf(x - dvd_x) < LOGO_R_WIDTH && fabsf(y - dvd_y) < LOGO_R_HEIGHT) {
            // Set the LED color to the current hue
            rgb_matrix_set_color(i, dvd_color[0], dvd_color[1], dvd_color[2]);
        } else {
            // Turn off other LEDs
            rgb_matrix_set_color(i, 0, 0, 0);
        }
    }

    return rgb_matrix_check_finished_leds(led_max);
}
