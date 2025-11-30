#include "assets_manager.h"
#include "ecs.h"
#include "raylib.h"
#include "raymath.h"
#include "settings.h"
#include "timers.h"
#include "types.h"
#include <corecrt_search.h>

#define PLAYER_TILE_1 (Rectangle){4, 400, 65, 65}
#define PLAYER_TILE_2 (Rectangle){4 + 66, 400, 65, 65}
#define PLAYER_TILE_3 (Rectangle){4 + (66 * 2), 400, 65, 65}

#define PLAYER_SIZE (TILE_SIZE << 1)
#define PLAYER_HALFSIZE (PLAYER_SIZE >> 1)

const Rectangle PLAYER_TILES[3] = {PLAYER_TILE_1, PLAYER_TILE_2, PLAYER_TILE_3};

void InitPlayer(World_t *world) {
  Entity_t *playerEntity = CreateEntity(world);

  AddComponentCpy(playerEntity, ((Transform2D_t){(Vector2){SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5}, 0, (Vector2){1, 1}}), Transform2D_t);
  AddComponentCpy(playerEntity, CreateNewTimer(), Timer_t);
  AddComponent(playerEntity, NULL, PLAYER); // Component used as TAG
}

void UpdatePlayer(World_t *world) {
  Vector2 direction = Vector2Zero();

  if (IsKeyDown(KEY_D)) direction.x = 1;
  else if (IsKeyDown(KEY_A)) direction.x = -1;
  if (IsKeyDown(KEY_W)) direction.y = -1;
  else if (IsKeyDown(KEY_S)) direction.y = 1;

  Entity_t *entity = GetEntityWithTypes(world, STR(PLAYER));

  if (entity == NULL) return;

  Transform2D_t *transform = GetComponentFromEntity(entity, Transform2D_t)->item;

  direction = Vector2Normalize(direction);

  transform->translation.x += (5 * DELTA_TIME) * direction.x;
  transform->translation.y += (10 * DELTA_TIME) * direction.y;

  if (transform->translation.x + PLAYER_HALFSIZE > SCREEN_WIDTH) {
    transform->translation.x = SCREEN_WIDTH - PLAYER_HALFSIZE;
  } else if (transform->translation.x - PLAYER_HALFSIZE < 0) {
    transform->translation.x = 0 + PLAYER_HALFSIZE;
  }

  if (transform->translation.y + PLAYER_HALFSIZE > SCREEN_HEIGHT - UI_BAR_HEIGHT) {
    transform->translation.y = SCREEN_HEIGHT - UI_BAR_HEIGHT - PLAYER_HALFSIZE;
  } else if (transform->translation.y - PLAYER_HALFSIZE < 0) {
    transform->translation.y = 0 + PLAYER_HALFSIZE;
  }
}

void DrawPlayer(World_t *world) {
  Entity_t *entity = GetEntityWithTypes(world, STR(PLAYER));

  if (entity == NULL) return;

  Timer_t *timer = GetComponentFromEntity(entity, Timer_t)->item;
  Transform2D_t *transform = GetComponentFromEntity(entity, Transform2D_t)->item;

  int frameIndex = GetAnimationFrame(timer, 3, 1.0 / 12);

  DrawTexturePro(*GetAssetFromName("GameAtlas", Texture2D),
                 PLAYER_TILES[frameIndex],
                 (Rectangle){transform->translation.x, transform->translation.y, PLAYER_SIZE * transform->scale.x, PLAYER_SIZE * transform->scale.y},
                 (Vector2){PLAYER_SIZE * 0.5, PLAYER_SIZE * 0.5}, transform->rotation, WHITE);
}