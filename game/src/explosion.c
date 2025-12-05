#include "assets_manager.h"
#include "ecs.h"
#include "raylib.h"
#include "raymath.h"
#include "settings.h"
#include "timers.h"
#include "types.h"
#include "utils.h"
#include <corecrt_search.h>
#include <stdbool.h>

const Rectangle EXPLOSION_TILES[5] = {EXPLOSION_TILE_1, EXPLOSION_TILE_2, EXPLOSION_TILE_3, EXPLOSION_TILE_4, EXPLOSION_TILE_5};

void InitExplosions(World_t *world) {
  Texture2D *atlas = GetAssetFromName("GameAtlas", Texture2D);

  for (int i = 0; i < ISLAND_NUMBER; i++) {
    Entity_t *entity = CreateEntity(world);
    entity->enabled = false;
    AddComponentCpy(entity, ((Transform2D_t){Vector2Zero(), 0, Vector2One()}), Transform2D_t);
    AddComponentCpy(entity, ((Sprite_t){EXPLOSION_TILE_1, atlas, TILE_SIZE}), Sprite_t);
    AddComponentCpy(entity, ((Animation_t){CreateNewTimer(), 1.0 / 12, 5, EXPLOSION_TILES}), Animation_t);
    AddComponentCpy(entity, CreateNewTimer(), Timer_t);
    AddComponent(entity, NULL, EXPLOSION);
  }
}

void UpdateExplosion(World_t *world) {
  aiv_vector_t search = GetEntitiesWithTypes(world, STR(EXPLOSION));

  for (int i = 0; i < search.count; i++) {
    Entity_t *entity = search.items[i];
    if (!entity->enabled) continue;

    Transform2D_t *transform = GetComponentFromEntity(entity, Transform2D_t)->item;
    transform->translation.y += TO_GAME_UNIT(2) * DELTA_TIME;

    Timer_t *timer = GetComponentFromEntity(entity, Timer_t)->item;
    Animation_t *animation = GetComponentFromEntity(entity, Animation_t)->item;

    if (GetTimerTime(timer) > animation->frameDuration * animation->frames) {
      entity->enabled = false;
    }
  }
}

void EnableExplosion(World_t *world, const Transform2D_t *parentTransform, bool big) {
  aiv_vector_t search = GetEntitiesWithTypes(world, STR(EXPLOSION));

  for (int i = 0; i < search.count; i++) {
    Entity_t *entity = search.items[i];
    if (entity->enabled) continue;

    Sprite_t *sprite = GetComponentFromEntity(entity, Sprite_t)->item;
    if (big) {
      sprite->size = TILE_SIZE << 1;
    } else {
      sprite->size = TILE_SIZE;
    }
    Timer_t *timer = GetComponentFromEntity(entity, Timer_t)->item;
    CycleTimerTime(timer);
    Transform2D_t *transform = GetComponentFromEntity(entity, Transform2D_t)->item;
    transform->translation = parentTransform->translation;
    entity->enabled = true;
    break;
  }
}
