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

// Snake node structure
typedef struct {
    int x;
    int y;
} Position;

// Game state
static int      tick = 0;
static Position snake[GRID_SIZE]; // Snake body
static int      snake_length;
static int      direction;
static Position food;
static bool     game_over;
static bool     game_over_toggle = false;

// Utility function to convert (x, y) to index
int position_to_index(int x, int y) {
    int   nearest = 0;
    float sq_dist = powf((float)g_led_config.point[0].x - x * COORD_SCALE_X, 2.0f) + powf((float)g_led_config.point[0].y - y * COORD_SCALE_Y, 2.0f);
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        float sq_dist_cmp = powf((float)g_led_config.point[i].x - x * COORD_SCALE_X, 2.0f) + powf((float)g_led_config.point[i].y - y * COORD_SCALE_Y, 2.0f);
        if (sq_dist_cmp < sq_dist) {
            nearest = i;
            sq_dist = sq_dist_cmp;
            if (sq_dist_cmp < 1) break;
        }
    }
    return nearest;
}

// Place food at a random position
void place_food(void) {
    bool valid_position = false;
    while (!valid_position) {
        food.x         = rand() % GRID_WIDTH;
        food.y         = rand() % GRID_HEIGHT;
        valid_position = true;
        // Ensure food does not spawn on the snake
        for (int i = 0; i < snake_length; i++) {
            if (snake[i].x == food.x && snake[i].y == food.y) {
                valid_position = false;
                break;
            }
        }
    }
}

// Reset the game
void reset_game(void) {
    snake_length     = 3;
    direction        = RIGHT;
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

static bool snake_ai(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    if (params->init) {
        reset_game();
    }

    tick++;

    if (game_over && tick % 20 == 0) {
        if (game_over_toggle) rgb_matrix_set_color_all(128, 0, 0);
        if (!game_over_toggle) rgb_matrix_set_color_all(0, 0, 0);
        game_over_toggle = !game_over_toggle;
    }

    if (tick % 100 == 0) {
        if (game_over) {
            reset_game();
        }

        // AI to move snake towards the food
        Position head = snake[0];
        if (food.x > head.x)
            direction = direction != LEFT ? RIGHT : UP;
        else if (food.x < head.x)
            direction = direction != RIGHT ? LEFT : DOWN;
        else if (food.y > head.y)
            direction = direction != UP ? DOWN : LEFT;
        else if (food.y < head.y)
            direction = direction != DOWN ? UP : RIGHT;

        // Shift the body
        for (int i = snake_length; i > 0; i--) {
            snake[i] = snake[i - 1];
        }

        // Move the head in the current direction
        if (direction == UP)
            snake[0].y--;
        else if (direction == DOWN)
            snake[0].y++;
        else if (direction == LEFT)
            snake[0].x--;
        else if (direction == RIGHT)
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
        // Check if food is eaten
        if (snake[0].x == food.x && snake[0].y == food.y) {
            snake_length++;
            place_food();
        }

        rgb_matrix_set_color_all(0, 0, 0);
        // Render snake
        for (int i = 0; i < snake_length; i++) {
            int index = position_to_index(snake[i].x, snake[i].y);
            rgb_matrix_set_color(index, 0, 255, 0); // Green snake
        }
        // Render food
        int food_index = position_to_index(food.x, food.y);
        rgb_matrix_set_color(food_index, 255, 0, 0); // Red food
    }
    return rgb_matrix_check_finished_leds(led_max);
}
