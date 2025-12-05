#ifndef SETTINGS_H
#define SETTINGS_H

//#define DEBUG_BUILD

// LAYERMASK
#define PLAYER_MASK 1
#define ENEMY_MASK 2
#define PLAYER_BULLET_MASK 4
#define ENEMY_BULLET_MASK 8

// UI
#define UI_BAR_HEIGHT (76 << (SCALE - 1))
#define LIVE_ICON (Rectangle){136, 202, 32, 32}

// GAME
#define TARGET_FRAMERATE 60
#define DELTA_MULTIPLIER 100
#define DELTA_TIME (GetFrameTime() * DELTA_MULTIPLIER)
#define SCREEN_HEIGHT (480 << (SCALE - 1))
#define SCREEN_WIDTH (640 << (SCALE - 1))
#define TILE_SIZE (32 << (SCALE - 1))
#define KEY_COLOR (Color){0, 67, 171, 255}
#define TO_GAME_UNIT(value) (value << (SCALE - 1))
#define SCALE 2

// PLAYER
#define PLAYER_TILE_1 (Rectangle){4, 400, 65, 65}
#define PLAYER_TILE_2 (Rectangle){4 + 66, 400, 65, 65}
#define PLAYER_TILE_3 (Rectangle){4 + (66 * 2), 400, 65, 65}

#define PLAYER_SIZE (TILE_SIZE << 1)
#define PLAYER_HALFSIZE (PLAYER_SIZE >> 1)
#define PLAYER_HEALTH 10
#define PLAYER_LIVES 3

#define SHOOT_COOLDOWN 0.25f

// BG
#define ISLAND_1 (Rectangle){103, 499, 64, 64}
#define ISLAND_2 (Rectangle){103 + 65, 499, 64, 64}
#define ISLAND_3 (Rectangle){103 + (65 * 2), 499, 64, 64}

#define WATER_TILE_1 (Rectangle){268, 367, 32, 32}
#define WATER_TILE_2 (Rectangle){268 + 33, 367, 32, 32}

#define ISLAND_SIZE (TILE_SIZE << 1)

#define ISLAND_NUMBER 5
#define ISLAND_DISTANCE 150 // WARNING: DISTANCE * (NUMBER-1) CAN'T GO OVER SCREEN_HEIGHT

// BULLETS
#define BULLET_POOL 50

#define BULLET_TILE (Rectangle){37, 169, 32, 32}

// ENEMIES
#define SHOOT_COOLDOWN_BOSS 0.25f

#define BOSS_HEALTH 30
#define ENEMY_POOL 100
#define ENEMY_SPAWN_RATE 0.1

#define BIG_ENEMY_TILE_1 (Rectangle){268, 400, 65, 65}
#define BIG_ENEMY_TILE_2 (Rectangle){268 + 66, 400, 65, 65}
#define BIG_ENEMY_TILE_3 (Rectangle){268 + (66 * 2), 400, 65, 65}

#define ENEMY_1_TILE_1 (Rectangle){4, 466, 32, 32}
#define ENEMY_1_TILE_2 (Rectangle){4 + 33, 466, 32, 32}
#define ENEMY_1_TILE_3 (Rectangle){4 + (33 * 2), 466, 32, 32}

#define ENEMY_2_TILE_1 (Rectangle){103, 466, 32, 32}
#define ENEMY_2_TILE_2 (Rectangle){103 + 33, 466, 32, 32}
#define ENEMY_2_TILE_3 (Rectangle){103 + (33 * 2), 466, 32, 32}

// EXPLOSIONS
#define EXPLOSIONS_POOL 10
#define EXPLOSION_TILE_1 (Rectangle){70, 169, 32, 32}
#define EXPLOSION_TILE_2 (Rectangle){70 + 33, 169, 32, 32}
#define EXPLOSION_TILE_3 (Rectangle){70 + (33 * 2), 169, 32, 32}
#define EXPLOSION_TILE_4 (Rectangle){70 + (33 * 3), 169, 32, 32}
#define EXPLOSION_TILE_5 (Rectangle){70 + (33 * 4), 169, 32, 32}

#endif