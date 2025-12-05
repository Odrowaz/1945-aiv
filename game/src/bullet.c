#include "assets_manager.h"
#include "ecs.h"
#include "raylib.h"
#include "raymath.h"
#include "settings.h"
#include "types.h"
#include <corecrt_search.h>
#include <math.h>
#include <stdbool.h>

void InitBullets(World_t *world) {
  Texture2D *atlas = GetAssetFromName("GameAtlas", Texture2D);

  for (int i = 0; i < BULLET_POOL; i++) {
    Entity_t *bullet = CreateEntity(world);
    bullet->enabled = false;

    AddComponentCpy(bullet, ((Transform2D_t){Vector2Zero(), 0, Vector2One()}), Transform2D_t);
    AddComponentCpy(bullet, ((Sprite_t){BULLET_TILE, atlas, TILE_SIZE}), Sprite_t);
    AddComponentCpy(bullet, ((Collider_t){(Rectangle){-(TILE_SIZE * 0.5), -(TILE_SIZE * 0.5), TILE_SIZE, TILE_SIZE}, false, 0, 0}), Collider_t);
    AddComponent(bullet, NULL, BULLET);
    // ADD COLLIDER;
  }
}

void ShootBullet(World_t *world, Transform2D_t *transform, unsigned int layermask, unsigned int collisionmask, float rotation, int bullets, int startIndex, int offset_y) {
  aiv_vector_t search = GetEntitiesWithTypes(world, STR(BULLET));
  int count = startIndex;
  for (int i = 0; i < search.count; i++) {
    Entity_t *bullet = search.items[i];
    if (bullet->enabled) continue;

    Transform2D_t *bulletTransform = GetComponentFromEntity(bullet, Transform2D_t)->item;
    Collider_t *collider = GetComponentFromEntity(bullet, Collider_t)->item;

    collider->collisionmask = collisionmask;
    collider->layermask = layermask;

    bulletTransform->rotation = rotation;
    bulletTransform->translation = transform->translation;
    bulletTransform->translation.y = bulletTransform->translation.y + TO_GAME_UNIT(offset_y);

    bullet->enabled = true;

    switch (count) {
    case 0:
      bulletTransform->rotation = rotation;
      break;
    case 1:
      bulletTransform->rotation = rotation + 12.5f;
      break;
    case 2:
      bulletTransform->rotation = rotation - 12.5f;
      break;
    default:
      break;
    }

    count++;
    if (count == bullets) {
      break;
    }
  }
}

void UpdateBullet(World_t *world) {
  aiv_vector_t search = GetEntitiesWithTypes(world, STR(BULLET));

  for (int i = 0; i < search.count; i++) {
    Entity_t *entity = search.items[i];
    if (!entity->enabled) continue;

    Transform2D_t *transform = GetComponentFromEntity(entity, Transform2D_t)->item;
    Collider_t *collider = GetComponentFromEntity(entity, Collider_t)->item;

    if (collider->hit) {
      entity->enabled = false;
      collider->hit = false;
    }

    Vector2 direction = {cosf(DEG2RAD * (transform->rotation - 90)), sinf(DEG2RAD * (transform->rotation - 90))};

    direction = Vector2Scale(direction, TO_GAME_UNIT(15) * DELTA_TIME);

    transform->translation = Vector2Add(transform->translation, direction);

    if (transform->translation.y + (TILE_SIZE >> 1) < 0) {
      entity->enabled = false;
    } else if (transform->translation.y - (TILE_SIZE >> 1) > SCREEN_HEIGHT) {
      entity->enabled = false;
    }
  }
}