#ifndef CONFIG_H
#define CONFIG_H

// Display
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define FPS_DELAY 17
#define ANIMATION_LOOP 4

// Map & size
#define MAP_SIZE 600
#define CASE_SIZE 120
#define CASE_OFFSET_X (WINDOW_WIDTH - MAP_SIZE) / 2 + CASE_SIZE / 6
#define CASE_OFFSET_Y (WINDOW_HEIGHT - MAP_SIZE) / 2 + CASE_SIZE / 6
#define SPRITE_SIZE CASE_SIZE / 2 + 20

// Difficulty : DIFFICULTY_K * exp((-(float)game->score) / DIFFICULTY_T)
#define FIREBALL_MOVELOOP 1 // Number of loop to move fireball (Don't change)
#define FIREBALL_SPEED 7   // Speed of fireball
#define OBSTACLE_WARNING 50 // Number of loop to show warning
#define DIFFICULTY_K 100   // K value of difficulty
#define DIFFICULTY_T 45    // T value of difficulty

// Other
#define WIDGET_LENGTH 48
#define END_SCORE_SIZE 20
#define SCORE_SIZE 25
#define MONEY_SIZE 20
#define PLAYER_MOVE 10
#define SKIN_W_H 120
#define SKIN_X_Y WINDOW_WIDTH / 2 - 60
#define FONT "../asset/police/arcade.ttf"
#define INTRO true

// Volume
#define VOLUME_MUSIC 75
#define VOLUME_COIN 65
#define VOLUME_FIREBALL 10
#define VOLUME_BUTTON 50
#define VOLUME_STEP 80
#define VOLUME_BACKGROUND 20

#endif
