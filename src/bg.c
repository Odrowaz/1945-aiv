
#include "assets_manager.h"
#include "ecs.h"
#include "settings.h"
#include "timers.h"
#include "utils.h"
#include <stdbool.h>

#define ISLAND_1 (Rectangle){103, 499, 64, 64}
#define ISLAND_2 (Rectangle){103 + 65, 499, 64, 64}
#define ISLAND_3 (Rectangle){103 + (65 * 2), 499, 64, 64}

#define WATER_TILE_1 (Rectangle){268, 367, 32, 32}
#define WATER_TILE_2 (Rectangle){268 + 33, 367, 32, 32}

#define ISLAND_SIZE (TILE_SIZE << 1)

void InitBg(world_t *world) {
  entity_t *bg = CreateEntity(world);

  AddComponentCpy(bg, ((ScrollingBG){0, GetAssetFromName("GameAtlas", Texture2D)}), ScrollingBG);
  AddComponentCpy(bg, CreateNewTimer(), Timer);

  AddComponentCpy(CreateEntity(world), ((ScrollingIslandBG){rand_range((ISLAND_SIZE / 2), SCREEN_WIDTH - (ISLAND_SIZE / 2)), 0, GetAssetFromName("GameAtlas", Texture2D), rand_range(0, 360), ISLAND_2, rand_range(0, 1)}), ScrollingIslandBG);
  AddComponentCpy(CreateEntity(world), ((ScrollingIslandBG){rand_range((ISLAND_SIZE / 2), SCREEN_WIDTH - (ISLAND_SIZE / 2)), 150, GetAssetFromName("GameAtlas", Texture2D), rand_range(0, 360), ISLAND_1, rand_range(0, 1)}), ScrollingIslandBG);
  AddComponentCpy(CreateEntity(world), ((ScrollingIslandBG){rand_range((ISLAND_SIZE / 2), SCREEN_WIDTH - (ISLAND_SIZE / 2)), 250, GetAssetFromName("GameAtlas", Texture2D), rand_range(0, 360), ISLAND_3, rand_range(0, 1)}), ScrollingIslandBG);
  AddComponentCpy(CreateEntity(world), ((ScrollingIslandBG){rand_range((ISLAND_SIZE / 2), SCREEN_WIDTH - (ISLAND_SIZE / 2)), 350, GetAssetFromName("GameAtlas", Texture2D), rand_range(0, 360), ISLAND_1, rand_range(0, 1)}), ScrollingIslandBG);
  AddComponentCpy(CreateEntity(world), ((ScrollingIslandBG){rand_range((ISLAND_SIZE / 2), SCREEN_WIDTH - (ISLAND_SIZE / 2)), 420, GetAssetFromName("GameAtlas", Texture2D), rand_range(0, 360), ISLAND_2, rand_range(0, 1)}), ScrollingIslandBG);
  AddComponentCpy(CreateEntity(world), ((ScrollingIslandBG){rand_range((ISLAND_SIZE / 2), SCREEN_WIDTH - (ISLAND_SIZE / 2)), 500, GetAssetFromName("GameAtlas", Texture2D), rand_range(0, 360), ISLAND_3, rand_range(0, 1)}), ScrollingIslandBG);
  AddComponentCpy(CreateEntity(world), ((ScrollingIslandBG){rand_range((ISLAND_SIZE / 2), SCREEN_WIDTH - (ISLAND_SIZE / 2)), 600, GetAssetFromName("GameAtlas", Texture2D), rand_range(0, 360), ISLAND_3, rand_range(0, 1)}), ScrollingIslandBG);
}

void UpdateBg(world_t *world) {
  aiv_vector_t search = GetEntitiesWithTypes(world, STR(ScrollingBG));

  for (int i = 0; i < search.count; i++) {

    entity_t *entity = search.items[i];
    ScrollingBG *bg = GetComponentFromEntity(entity, ScrollingBG)->item;

    bg->offset_y += 2;
    bg->offset_y %= SCREEN_HEIGHT;
  }

  search = GetEntitiesWithTypes(world, STR(ScrollingIslandBG));

  for (int i = 0; i < search.count; i++) {

    entity_t *entity = search.items[i];
    ScrollingIslandBG *island = GetComponentFromEntity(entity, ScrollingIslandBG)->item;

    island->y += 2;
    if (island->y > (SCREEN_HEIGHT + (ISLAND_SIZE / 2))) {
      island->y = -(ISLAND_SIZE / 2);
      island->x = rand_range((ISLAND_SIZE / 2), SCREEN_WIDTH - (ISLAND_SIZE / 2));
      island->enabled = rand_range(0, 1);
      island->rotation = rand_range(0, 360);
    }
  }
}

void DrawBg(world_t *world) {
  aiv_vector_t search = GetEntitiesWithTypes(world, STR(ScrollingBG), STR(Timer));

  for (int i = 0; i < search.count; i++) {

    entity_t *entity = search.items[i];

    Timer *timer = GetComponentFromEntity(entity, Timer)->item;
    ScrollingBG *bg = GetComponentFromEntity(entity, ScrollingBG)->item;

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

void DrawIslandBg(world_t *world) {
  aiv_vector_t search = GetEntitiesWithTypes(world, STR(ScrollingIslandBG));

  for (int i = 0; i < search.count; i++) {

    entity_t *entity = search.items[i];

    ScrollingIslandBG *bg = GetComponentFromEntity(entity, ScrollingIslandBG)->item;

    if (bg->enabled) {
      DrawTexturePro(*bg->atlas, bg->tile,
                     (Rectangle){bg->x, bg->y, ISLAND_SIZE, ISLAND_SIZE},
                     (Vector2){ISLAND_SIZE * 0.5, ISLAND_SIZE * 0.5}, bg->rotation, WHITE);
    }
  }
}
