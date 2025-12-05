#include "assets_manager.h"
#include "bullet.h"
#include "ecs.h"
#include "explosion.h"
#include "raylib.h"
#include "raymath.h"
#include "settings.h"
#include "timers.h"
#include "types.h"
#include <corecrt_search.h>
#include <stdbool.h>


const Rectangle PLAYER_TILES[3] = {PLAYER_TILE_1, PLAYER_TILE_2, PLAYER_TILE_3};

void InitPlayer(World_t *world) {
  Entity_t *playerEntity = CreateEntity(world);

  AddComponentCpy(playerEntity, ((Transform2D_t){(Vector2){SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1}, 0, (Vector2){1, 1}}), Transform2D_t);
  AddComponentCpy(playerEntity, ((Sprite_t){PLAYER_TILES[0], GetAssetFromName("GameAtlas", Texture2D), PLAYER_SIZE}), Sprite_t);
  AddComponentCpy(playerEntity, ((Animation_t){CreateNewTimer(), 1.0 / 12, 3, PLAYER_TILES}), Animation_t);
  AddComponentCpy(playerEntity, ((Collider_t){(Rectangle){-PLAYER_HALFSIZE, -PLAYER_HALFSIZE, PLAYER_SIZE, PLAYER_SIZE}, false, PLAYER_MASK, ENEMY_BULLET_MASK | ENEMY_MASK}), Collider_t);
  AddComponentCpyTag(playerEntity, CreateNewTimer(), Timer_t, BULLET_TIMER);
  AddComponentCpyTag(playerEntity, PLAYER_HEALTH, int, HEALTH);
  AddComponentCpyTag(playerEntity, PLAYER_LIVES, int, LIVES);
  AddComponentCpyTag(playerEntity, 0, int, SCORE);
  AddComponent(playerEntity, NULL, PLAYER); // Component used as TAG
}

void UpdatePlayer(World_t *world) {
  Vector2 direction = Vector2Zero();

  if (IsKeyDown(KEY_D)) direction.x = 1;
  else if (IsKeyDown(KEY_A)) direction.x = -1;
  if (IsKeyDown(KEY_W)) direction.y = -1;
  else if (IsKeyDown(KEY_S)) direction.y = 1;

  Entity_t *entity = GetEntityWithTypes(world, STR(PLAYER));

  if (!entity->enabled) return;

  int *health = GetComponentFromEntity(entity, HEALTH)->item;
  int *lives = GetComponentFromEntity(entity, LIVES)->item;
  Collider_t *collider = GetComponentFromEntity(entity, Collider_t)->item;
  Transform2D_t *transform = GetComponentFromEntity(entity, Transform2D_t)->item;
  Timer_t *timer = GetComponentFromEntity(entity, BULLET_TIMER)->item;

  if (collider->hit) {
    collider->hit = false;
    (*health)--;
    if (*health == 0) {
      (*lives)--;
      if (*lives == 0) {
        // GAME OVER
        entity->enabled = false;
        EnableExplosion(world, transform, true);
      }
      (*health) = PLAYER_HEALTH;
    }
  }

  direction = Vector2Normalize(direction);

  transform->translation.x += TO_GAME_UNIT(4) * DELTA_TIME * direction.x;
  transform->translation.y += TO_GAME_UNIT(5) * DELTA_TIME * direction.y;

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

  if (IsKeyDown(KEY_SPACE) && GetTimerTime(timer) >= SHOOT_COOLDOWN) {
    CycleTimerTime(timer);
    ShootBullet(world, transform, PLAYER_BULLET_MASK, ENEMY_MASK, 0, 3, 1, -20);
  }
}
