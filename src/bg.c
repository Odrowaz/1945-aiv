#include "assets_manager.h"
#include "ecs.h"
#include "raylib.h"
#include "settings.h"
#include "timers.h"
#include "types.h"
#include "utils.h"
#include <stdbool.h>

#define ISLAND_1 (Rectangle){103, 499, 64, 64}
#define ISLAND_2 (Rectangle){103 + 65, 499, 64, 64}
#define ISLAND_3 (Rectangle){103 + (65 * 2), 499, 64, 64}

#define WATER_TILE_1 (Rectangle){268, 367, 32, 32}
#define WATER_TILE_2 (Rectangle){268 + 33, 367, 32, 32}

#define ISLAND_SIZE (TILE_SIZE << 1)

#define ISLAND_NUMBER 5
#define ISLAND_DISTANCE 150 // WARNING: DISTANCE * (NUMBER-1) CAN'T GO OVER SCREEN_HEIGHT

const Rectangle ISLAND_TILES[3] = {ISLAND_1, ISLAND_2, ISLAND_3};

void InitBg(World_t *world) {
  Entity_t *bg = CreateEntity(world);

  AddComponentCpy(bg, ((ScrollingBG_t){0, GetAssetFromName("GameAtlas", Texture2D)}), ScrollingBG_t);
  AddComponentCpy(bg, CreateNewTimer(), Timer_t);

  for (int i = 0; i < ISLAND_NUMBER; i++) {
    Entity_t *island = CreateEntity(world);
    AddComponentCpy(island, ((Transform2D_t){(Vector2){rand_range((ISLAND_SIZE / 2), SCREEN_WIDTH - (ISLAND_SIZE / 2)), i * ISLAND_DISTANCE}, rand_range(0, 360), (Vector2){1, 1}}), Transform2D_t);
    AddComponentCpy(island, ((ScrollingIslandBG_t){GetAssetFromName("GameAtlas", Texture2D), ISLAND_TILES[rand_range(0, 2)], rand_range(0, 1)}), ScrollingIslandBG_t);
  }
}

void UpdateBg(World_t *world) {
  aiv_vector_t search = GetEntitiesWithTypes(world, STR(ScrollingBG_t));

  for (int i = 0; i < search.count; i++) {

    Entity_t *entity = search.items[i];
    ScrollingBG_t *bg = GetComponentFromEntity(entity, ScrollingBG_t)->item;

    bg->offset_y += 2;
    bg->offset_y %= SCREEN_HEIGHT;
  }

  search = GetEntitiesWithTypes(world, STR(ScrollingIslandBG_t), STR(Transform2D_t));

  for (int i = 0; i < search.count; i++) {

    Entity_t *entity = search.items[i];
    ScrollingIslandBG_t *island = GetComponentFromEntity(entity, ScrollingIslandBG_t)->item;
    Transform2D_t *islandTransform = GetComponentFromEntity(entity, Transform2D_t)->item;

    islandTransform->translation.y += 2;
    if (islandTransform->translation.y > (SCREEN_HEIGHT + (ISLAND_SIZE * 0.5))) {
      islandTransform->translation.y = -(ISLAND_SIZE * 0.5);
      islandTransform->translation.x = rand_range((ISLAND_SIZE / 2), SCREEN_WIDTH - (ISLAND_SIZE / 2));
      islandTransform->rotation = rand_range(0, 360);

      island->enabled = rand_range(0, 1);
      island->tile = ISLAND_TILES[rand_range(0, 2)];
    }
  }
}

void DrawBg(World_t *world) {
  aiv_vector_t search = GetEntitiesWithTypes(world, STR(ScrollingBG_t), STR(Timer_t));

  for (int i = 0; i < search.count; i++) {

    Entity_t *entity = search.items[i];

    Timer_t *timer = GetComponentFromEntity(entity, Timer_t)->item;
    ScrollingBG_t *bg = GetComponentFromEntity(entity, ScrollingBG_t)->item;

    int frameIndex = GetAnimationFrame(timer, 2, 1.0 / 1);
    for (int y = 0; y < SCREEN_HEIGHT + TILE_SIZE; y += TILE_SIZE) {
      int temp_y = (y + bg->offset_y);
      if (temp_y > SCREEN_HEIGHT) {
        temp_y = -TILE_SIZE + (temp_y - SCREEN_HEIGHT);
      }

      for (int x = 0; x < SCREEN_WIDTH; x += TILE_SIZE) {
        DrawTexturePro(*bg->atlas,
                       frameIndex == 1 ? WATER_TILE_1 : WATER_TILE_2,
                       (Rectangle){x, temp_y, TILE_SIZE, TILE_SIZE},
                       (Vector2){0, 0}, 0, WHITE);
      }
    }
  }
}

void DrawIslandBg(World_t *world) {
  aiv_vector_t search = GetEntitiesWithTypes(world, STR(ScrollingIslandBG_t), STR(Transform2D_t));

  for (int i = 0; i < search.count; i++) {

    Entity_t *entity = search.items[i];

    ScrollingIslandBG_t *island = GetComponentFromEntity(entity, ScrollingIslandBG_t)->item;
    Transform2D_t *islandTransform = GetComponentFromEntity(entity, Transform2D_t)->item;

    if (island->enabled) {
      DrawTexturePro(*island->atlas, island->tile,
                     (Rectangle){islandTransform->translation.x, islandTransform->translation.y, ISLAND_SIZE * islandTransform->scale.x, ISLAND_SIZE * islandTransform->scale.y},
                     (Vector2){ISLAND_SIZE * 0.5, ISLAND_SIZE * 0.5}, islandTransform->rotation, WHITE);
    }
  }
}
