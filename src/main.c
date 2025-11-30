#include "assets_manager.h"
#include "bg.h"
#include "ecs.h"
#include "raylib.h"
#include "types.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include "settings.h"
#include <time.h>

void LoadAllAssets() {
  const char *app_dir = GetApplicationDirectory();
  ChangeDirectory(app_dir);
  ChangeDirectory("..");

  Image atlas = LoadImage("assets/1945_atlas.bmp");

  AddNewAsset("GameAtlas", "assets/1945_atlas.bmp", TEXTURE);

  AddNewAsset("GameIcon", "assets/ui/life.png", IMAGE);
}

int main(void) {
  srand(time(NULL));

  char title[100];

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Loading...");
  SetTargetFPS(TARGET_FRAMERATE);
  LoadAllAssets();

  SetWindowIcon(*GetAssetFromName("GameIcon", Image));
  InitAudioDevice();

  size_t worldIndex = CreateWorld();

  RegisterSystem(InitBg, "INIT");
  RegisterSystem(UpdateBg, "UPDATE");
  RegisterSystem(DrawBg, "DRAW");
  RegisterSystem(DrawIslandBg, "DRAW");

  RunSystems(worldIndex, "INIT");

  while (!WindowShouldClose()) {
    float delta_time = GetFrameTime();
    int fps = GetFPS();
    sprintf(title, "1945 (Delta: %.4f - FPS: %d)", delta_time, fps);
    SetWindowTitle(title);

    RunSystems(worldIndex, "UPDATE");

    BeginDrawing();
    {
      ClearBackground(KEY_COLOR);
      RunSystems(worldIndex, "DRAW");
    }
    EndDrawing();
  }

  FreeAssets();
  DestroyWorld(worldIndex);
  DestroyECS();
  CloseWindow();

  return 0;
}
