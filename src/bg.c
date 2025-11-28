
#include "assets_manager.h"
#include "ecs.h"
#include "settings.h"
#include "timers.h"

#define ISLAND_1 (Rectangle){103, 499, 64, 64}
#define ISLAND_2 (Rectangle){103 + 64, 499, 64, 64}
#define ISLAND_3 (Rectangle){103 + (64 * 2), 499, 64, 64}

#define WATER_TILE_1 (Rectangle){268, 367, 32, 32}
#define WATER_TILE_2 (Rectangle){268 + 33, 367, 32, 32}

void initBg(world_t *world) {
  entity_t *bg = CreateEntity(world);

  ScrollingBG *scrollBg = malloc(sizeof(ScrollingBG));
  *scrollBg = (ScrollingBG){0, GetAssetFromName("GameAtlas", Texture2D)};

  Timer bgTimer = CreateNewTimer();

  AddComponent(bg, scrollBg, ScrollingBG);
  AddComponentCpy(bg, &bgTimer, Timer);
}

void DrawBg(world_t *world) {
  aiv_vector_t search = GetEntitiesWithTypes(world, STR(ScrollingBG), STR(Timer));

  for (int i = 0; i < search.count; i++) {

    entity_t *entity = search.items[i];

    Timer *timer = GetComponentFromEntity(entity, Timer)->item;
    ScrollingBG *bg = GetComponentFromEntity(entity, ScrollingBG)->item;

    bg->offset_y += 2;
    bg->offset_y %= SCREEN_HEIGHT;

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
  aiv_vector_t search = GetEntitiesWithTypes(world, STR(ScrollingBG), STR(Timer));

  for (int i = 0; i < search.count; i++) {

    entity_t *entity = search.items[i];

    Timer *timer = GetComponentFromEntity(entity, Timer)->item;
    ScrollingBG *bg = GetComponentFromEntity(entity, ScrollingBG)->item;

    DrawTexturePro(*bg->atlas, ISLAND_1,
                   (Rectangle){10, 40, TILE_SIZE << 2, TILE_SIZE << 2},
                   (Vector2){0, 0}, 0, WHITE);
  }
}
