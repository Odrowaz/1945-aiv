#include "assets_manager.h"
#include "ecs.h"
#include "raylib.h"
#include "settings.h"
#include <stdio.h>

void DrawUi(World_t *world) {

  DrawTexturePro(*GetAssetFromName("UIBottomBar", Texture2D),
                 ((Rectangle){0, 0, 640, 76}),
                 ((Rectangle){0, SCREEN_HEIGHT - UI_BAR_HEIGHT, SCREEN_WIDTH, UI_BAR_HEIGHT}),
                 (Vector2){0, 0}, 0, WHITE);

  Entity_t *player = GetEntityWithTypes(world, STR(PLAYER));

  if (player == NULL) return;

  int *health = GetComponentFromEntity(player, HEALTH)->item;
  int *lives = GetComponentFromEntity(player, LIVES)->item;
  int *score = GetComponentFromEntity(player, SCORE)->item;

  int padding_x = TO_GAME_UNIT(25);
  int padding_y = TO_GAME_UNIT(5);

  for (int i = 0; i < *lives; i++) {
    DrawTexturePro(*GetAssetFromName("GameAtlas", Texture2D),
                   LIVE_ICON,
                   ((Rectangle){0 + padding_x + (i * TILE_SIZE), SCREEN_HEIGHT - UI_BAR_HEIGHT + padding_y, TILE_SIZE, TILE_SIZE}),
                   (Vector2){0, 0}, 0, WHITE);
  }

  int healthBar = (100 * (*health)) / PLAYER_HEALTH;

  DrawRectangle(TO_GAME_UNIT(12), SCREEN_HEIGHT - UI_BAR_HEIGHT + TO_GAME_UNIT(45), (TO_GAME_UNIT(126) * healthBar) / 100, TO_GAME_UNIT(10), GREEN);

  char scoreText[20];

  sprintf(scoreText, "Score: %d", *score);

  DrawText(scoreText, TO_GAME_UNIT(230), SCREEN_HEIGHT - UI_BAR_HEIGHT + TO_GAME_UNIT(10), TO_GAME_UNIT(20), YELLOW);
}