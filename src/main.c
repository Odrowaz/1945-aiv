#include "aiv_vector.h"
#include "assets_manager.h"
#include "ecs.h"
#include "raylib.h"
#include "timers.h"
#include "types.h"
#include <math.h>
#include <stdio.h>

#define ISLAND_1 (Rectangle){103, 499, 64, 64}
#define ISLAND_2 (Rectangle){103 + 64, 499, 64, 64}
#define ISLAND_3 (Rectangle){103 + (64 * 2), 499, 64, 64}

#define WATER_TILE_1 (Rectangle){268, 367, 32, 32}
#define WATER_TILE_2 (Rectangle){268 + 33, 367, 32, 32}
#define TARGET_FRAMERATE 60
#define SCREEN_WIDTH (640 << 1)
#define SCREEN_HEIGHT (480 << 1)
#define TILE_SIZE (32 << 1)

void LoadAllAssets() {
  const char *app_dir = GetApplicationDirectory();
  ChangeDirectory(app_dir);
  ChangeDirectory("..");

  Image atlas = LoadImage("assets/1945_atlas.bmp");

  AddNewAsset("GameAtlas", "assets/1945_atlas.bmp", TEXTURE);

  AddNewAsset("GameIcon", "assets/ui/life.png", IMAGE);
}

void DrawBg(world_t *world) {
  world_t search = GetEntitiesWithTypes(world, STR(ScrollingBG), STR(Timer));

  for (int i = 0; i < search.entities.count; i++) {

    world_t entity_components =
        GetComponentsFromEntity(world, search.entities.items[i]);

    Timer *timer = GetComponentOfType(&entity_components, Timer)->item;
    ScrollingBG *bg = GetComponentOfType(&entity_components, ScrollingBG)->item;

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

void DrawIslandBg(ScrollingBG *bg, Timer *timer) {

  DrawTexturePro(*bg->atlas, ISLAND_1,
                 (Rectangle){10, 40, TILE_SIZE << 2, TILE_SIZE << 2},
                 (Vector2){0, 0}, 0, WHITE);
}

int main(void) {

  char title[100];

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Loading...");
  SetTargetFPS(TARGET_FRAMERATE);
  LoadAllAssets();

  SetWindowIcon(*GetAssetFromName("GameIcon", Image));
  InitAudioDevice();

  world_t world = CreateWorld();

  entity_t bg = CreateEntity(&world);

  ScrollingBG scrollBg = {0, GetAssetFromName("GameAtlas", Texture2D)};
  Timer bgTimer = CreateNewTimer();

  AddComponent(&world, &bgTimer, Timer, &bg);
  AddComponent(&world, &scrollBg, ScrollingBG, &bg);

  RegisterSystem(DrawBg, "DRAW");

  while (!WindowShouldClose()) {
    float delta_time = GetFrameTime();
    int fps = GetFPS();
    sprintf(title, "1945 (Delta: %.4f - FPS: %d)", delta_time, fps);
    SetWindowTitle(title);

    BeginDrawing();
    {
      ClearBackground(BLANK);
      RunSystems(&world, "DRAW");
      // DrawBg(&bg, &bgTimer);
      // DrawIslandBg(&bg, &bgTimer);
    }
    EndDrawing();
  }

  FreeAssets();
  CloseWindow();
  return 0;
}
