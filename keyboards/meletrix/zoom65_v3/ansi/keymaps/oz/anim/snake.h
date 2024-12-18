/* Auto Snake Game Effect */

#define GRID_WIDTH 16
#define GRID_HEIGHT 5
#define GRID_SIZE (GRID_WIDTH * GRID_HEIGHT)
#define COORD_SCALE_X 224 / GRID_WIDTH
#define COORD_SCALE_Y 64 / GRID_HEIGHT

// Directions
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#ifndef SNAKE_MIN_FRAMES
#    define SNAKE_MIN_FRAMES 10
#endif

#ifndef SNAKE_MAX_FRAMES
#    define SNAKE_MAX_FRAMES 100
#endif

// Snake node structure
typedef struct {
    int x;
    int y;
} Position;

// Game state
static int      snake_tick = 0;
static Position snake[GRID_SIZE]; // Snake body
static int      snake_length;
static int      snake_direction;
static Position snake_food;
static bool     game_over;
static bool     game_over_toggle = false;

float sq_dist_scaled(float ax, float ay, float bx, float by) {
    return powf(ax - bx * COORD_SCALE_X, 2) + powf(ay - by * COORD_SCALE_Y, 2);
}

// Utility function to convert (x, y) to index
int position_to_index(int x, int y) {
    int   nearest = 0;
    float sq_dist = sq_dist_scaled(g_led_config.point[0].x, g_led_config.point[0].y, x, y);
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        float sq_dist_cmp = sq_dist_scaled(g_led_config.point[i].x, g_led_config.point[i].y, x, y);
        if (sq_dist_cmp < sq_dist) {
            nearest = i;
            sq_dist = sq_dist_cmp;
            if (sq_dist_cmp < 1) break;
        }
    }
    return nearest;
}

// Place snake_food at a random position
void place_food(void) {
    bool valid_position = false;
    while (!valid_position) {
        snake_food.x   = rand() % GRID_WIDTH;
        snake_food.y   = rand() % GRID_HEIGHT;
        valid_position = true;
        // Ensure snake_food does not spawn on the snake
        for (int i = 0; i < snake_length; i++) {
            if (snake[i].x == snake_food.x && snake[i].y == snake_food.y) {
                valid_position = false;
                break;
            }
        }
    }
}

// Reset the game
void reset_game(void) {
    snake_length     = 3;
    snake_direction  = RIGHT;
    game_over        = false;
    game_over_toggle = false;

    // Initialize snake in the center
    for (int i = 0; i < snake_length; i++) {
        snake[i].x = GRID_WIDTH / 2 - i;
        snake[i].y = GRID_HEIGHT / 2;
    }

    place_food();
    rgb_matrix_set_color_all(0, 0, 0);
}

static bool AUTO_SNAKE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    HSV hsv      = rgb_matrix_config.hsv;
    int tickrate = SNAKE_MIN_FRAMES + ((255 - rgb_matrix_config.speed) * (SNAKE_MAX_FRAMES - SNAKE_MIN_FRAMES)) / 255;
    snake_tick++;

    if (params->init) {
        reset_game();
    }

    // compute snake and food colors
    RGB snake_color = rgb_matrix_hsv_to_rgb(hsv);
    hsv.h           = 0;
    RGB food_color  = rgb_matrix_hsv_to_rgb(hsv);

    // Render dead snake
    if (game_over)
        for (int i = 0; i < snake_length; i++) {
            int index = position_to_index(snake[i].x, snake[i].y);
            rgb_matrix_set_color(index, food_color.r, food_color.g, food_color.b);
        };

    if (snake_tick % tickrate == 0) {
        if (game_over) {
            reset_game();
        }

        // AI to move snake towards the food
        Position head = snake[0];
        if (snake_food.x > head.x)
            snake_direction = snake_direction != LEFT ? RIGHT : UP;
        else if (snake_food.x < head.x)
            snake_direction = snake_direction != RIGHT ? LEFT : DOWN;
        else if (snake_food.y > head.y)
            snake_direction = snake_direction != UP ? DOWN : LEFT;
        else if (snake_food.y < head.y)
            snake_direction = snake_direction != DOWN ? UP : RIGHT;

        // Shift the body
        for (int i = snake_length; i > 0; i--) {
            snake[i] = snake[i - 1];
        }

        // Move the head in the current snake_direction
        if (snake_direction == UP)
            snake[0].y--;
        else if (snake_direction == DOWN)
            snake[0].y++;
        else if (snake_direction == LEFT)
            snake[0].x--;
        else if (snake_direction == RIGHT)
            snake[0].x++;

        // Check for collisions with walls
        if (snake[0].x < 0 || snake[0].x >= GRID_WIDTH || snake[0].y < 0 || snake[0].y >= GRID_HEIGHT) {
            game_over = true;
            return false;
        }
        // Check for collisions with itself
        for (int i = 1; i < snake_length; i++) {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                game_over = true;
                return false;
            }
        }
        // Check if snake_food is eaten
        if (snake[0].x == snake_food.x && snake[0].y == snake_food.y) {
            snake_length++;
            place_food();
        }

        rgb_matrix_set_color_all(0, 0, 0);
        // Render snake
        for (int i = 0; i < snake_length; i++) {
            int index = position_to_index(snake[i].x, snake[i].y);
            rgb_matrix_set_color(index, snake_color.r, snake_color.g, snake_color.b);
        }
        // Render snake_food
        int index = position_to_index(snake_food.x, snake_food.y);
        rgb_matrix_set_color(index, food_color.r, food_color.g, food_color.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}
