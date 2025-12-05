#include "assets_manager.h"
#include "bullet.h"
#include "ecs.h"
#include "explosion.h"
#include "raylib.h"
#include "raymath.h"
#include "settings.h"
#include "timers.h"
#include "types.h"
#include "utils.h"
#include <math.h>
#include <stdbool.h>

const Rectangle ENEMY_1_TILES[3] = {ENEMY_1_TILE_1, ENEMY_1_TILE_2, ENEMY_1_TILE_3};
const Rectangle ENEMY_2_TILES[3] = {ENEMY_2_TILE_1, ENEMY_2_TILE_2, ENEMY_2_TILE_3};
const Rectangle BIG_ENEMY_TILES[3] = {BIG_ENEMY_TILE_1, BIG_ENEMY_TILE_2, BIG_ENEMY_TILE_3};

void InitEnemies(World_t *world) {
  Texture2D *atlas = GetAssetFromName("GameAtlas", Texture2D);
  Entity_t *enemySpawner = CreateEntity(world);
  AddComponentCpy(enemySpawner, CreateNewTimer(), Timer_t);
  AddComponent(enemySpawner, NULL, ENEMY_SPAWNER);
  AddComponentCpyTag(enemySpawner, 0, int, ENEMY_SPAWNED);
  AddComponentCpyTag(enemySpawner, false, bool, BOSS);

  for (int i = 0; i < BULLET_POOL; i++) {
    Entity_t *enemy = CreateEntity(world);
    enemy->enabled = false;

    AddComponentCpy(enemy, ((Transform2D_t){Vector2Zero(), 0, Vector2One()}), Transform2D_t);
    AddComponentCpy(enemy, ((Sprite_t){ENEMY_1_TILE_1, atlas, TILE_SIZE}), Sprite_t);
    AddComponentCpy(enemy, ((Animation_t){CreateNewTimer(), 1.0 / 12, 3, ENEMY_1_TILES}), Animation_t);
    AddComponentCpy(enemy, ((Collider_t){(Rectangle){-(TILE_SIZE * 0.5), -(TILE_SIZE * 0.5), TILE_SIZE, TILE_SIZE}, false, ENEMY_MASK, PLAYER_MASK | PLAYER_BULLET_MASK}), Collider_t);
    AddComponent(enemy, NULL, ENEMY);
  }

  Entity_t *enemy = CreateEntity(world);
  enemy->enabled = false;

  AddComponentCpy(enemy, ((Transform2D_t){(Vector2){SCREEN_WIDTH * 0.5, PLAYER_HALFSIZE}, 180, Vector2One()}), Transform2D_t);
  AddComponentCpy(enemy, ((Sprite_t){BIG_ENEMY_TILES[0], atlas, PLAYER_SIZE}), Sprite_t);
  AddComponentCpy(enemy, ((Animation_t){CreateNewTimer(), 1.0 / 12, 3, BIG_ENEMY_TILES}), Animation_t);
  AddComponentCpy(enemy, ((Collider_t){(Rectangle){-(PLAYER_SIZE * 0.5), -(PLAYER_SIZE * 0.5), PLAYER_SIZE, PLAYER_SIZE}, false, ENEMY_MASK, PLAYER_MASK | PLAYER_BULLET_MASK}), Collider_t);
  AddComponentCpy(enemy, CreateNewTimer(), Timer_t);
  AddComponentCpyTag(enemy, BOSS_HEALTH, int, HEALTH);
  AddComponent(enemy, NULL, BOSS_ENEMY);
}

void ManageSpawn(bool *isBossAlive, bool *didSpawn, Entity_t *entity, Timer_t *timer, Sprite_t *sprite, int *spawnedEntity, Transform2D_t *transform, World_t *world) {
  if (!(*isBossAlive) && !(*didSpawn) && !entity->enabled && GetTimerTime(timer) > ENEMY_SPAWN_RATE) {
    *spawnedEntity = (*spawnedEntity + 1) % ENEMY_POOL;
    CycleTimerTime(timer);
    (*didSpawn) = true;
    entity->enabled = true;
    transform->translation.y = -(sprite->size * 0.5);
    transform->translation.x = rand_range((sprite->size * 0.5), SCREEN_WIDTH - (sprite->size * 0.5));

    if (*spawnedEntity == 0) {
      *isBossAlive = true;
      Entity_t *boss = GetEntityWithTypes(world, STR(BOSS_ENEMY));
      int *health = GetComponentFromEntity(boss, HEALTH)->item;
      Timer_t *timer = GetComponentFromEntity(boss, Timer_t)->item;
      CycleTimerTime(timer);
      (*health) = BOSS_HEALTH;
      boss->enabled = true;
    }
  }
}

void UpdateEnemies(World_t *world) {
  Entity_t *enemySpawner = GetEntityWithTypes(world, STR(ENEMY_SPAWNER));
  bool *isBossAlive = GetComponentFromEntity(enemySpawner, BOSS)->item;
  Timer_t *timer = GetComponentFromEntity(enemySpawner, Timer_t)->item;
  int *spawnedEntity = GetComponentFromEntity(enemySpawner, ENEMY_SPAWNED)->item;
  bool didSpawn = false;

  aiv_vector_t search = GetEntitiesWithTypes(world, STR(ENEMY));

  for (int i = 0; i < search.count; i++) {
    Entity_t *entity = search.items[i];
    Transform2D_t *transform = GetComponentFromEntity(entity, Transform2D_t)->item;
    Sprite_t *sprite = GetComponentFromEntity(entity, Sprite_t)->item;

    ManageSpawn(isBossAlive, &didSpawn, entity, timer, sprite, spawnedEntity, transform, world);

    if (!entity->enabled) continue;

    Collider_t *collider = GetComponentFromEntity(entity, Collider_t)->item;

    if (collider->hit) {

      entity->enabled = false;
      collider->hit = false;
      EnableExplosion(world, transform, false);

      Entity_t *player = GetEntityWithTypes(world, STR(PLAYER));
      int *score = GetComponentFromEntity(player, SCORE)->item;

      (*score) += 50;

      return;
    }

    Vector2 direction = {0, 1};

    direction = Vector2Scale(direction, TO_GAME_UNIT(3) * DELTA_TIME);

    transform->translation = Vector2Add(transform->translation, direction);

    if (transform->translation.y - (sprite->size * 0.5) > SCREEN_HEIGHT) {
      entity->enabled = false;
    }
  }
}

void UpdateBoss(World_t *world) {
  Entity_t *entity = GetEntityWithTypes(world, STR(BOSS_ENEMY));

  Transform2D_t *transform = GetComponentFromEntity(entity, Transform2D_t)->item;
  int *health = GetComponentFromEntity(entity, HEALTH)->item;

  if (!entity->enabled) return;

  Collider_t *collider = GetComponentFromEntity(entity, Collider_t)->item;
  Entity_t *player = GetEntityWithTypes(world, STR(PLAYER));
  Transform2D_t *playerTransform = GetComponentFromEntity(player, Transform2D_t)->item;

  if (collider->hit) {
    collider->hit = false;
    (*health)--;

    if (*health == 0) {
      entity->enabled = false;
      EnableExplosion(world, transform, true);
      int *score = GetComponentFromEntity(player, SCORE)->item;

      (*score) += 200;
      Entity_t *enemySpawner = GetEntityWithTypes(world, STR(ENEMY_SPAWNER));
      bool *isBossAlive = GetComponentFromEntity(enemySpawner, BOSS)->item;
      *isBossAlive = false;
    }

    return;
  }

  Timer_t *timer = GetComponentFromEntity(entity, Timer_t)->item;

  if (GetTimerTime(timer) >= SHOOT_COOLDOWN_BOSS && player->enabled) {
    CycleTimerTime(timer);
    ShootBullet(world, transform, ENEMY_BULLET_MASK, PLAYER_MASK, 180, 1, 0, 20);
  }

  Vector2 direction = Vector2Normalize(Vector2Subtract(playerTransform->translation, transform->translation));
  direction.y = 0;
  direction.x = direction.x > 0.01 ? 1 : direction.x < -0.01 ? -1
                                                             : 0;

  Vector2 velocity = Vector2Scale(direction, TO_GAME_UNIT(2) * DELTA_TIME);

  transform->translation = Vector2Add(transform->translation, velocity);

  if (transform->translation.x + PLAYER_HALFSIZE > SCREEN_WIDTH) {
    transform->translation.x = SCREEN_WIDTH - PLAYER_HALFSIZE;
  } else if (transform->translation.x - PLAYER_HALFSIZE < 0) {
    transform->translation.x = 0 + PLAYER_HALFSIZE;
  }
}
