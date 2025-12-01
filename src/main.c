#include "assets_manager.h"
#include "bg.h"
#include "ecs.h"
#include "player.h"
#include "raylib.h"
#include "settings.h"
#include "types.h"
#include "ui.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

void LoadAllAssets() {
  const char *app_dir = GetApplicationDirectory();
  ChangeDirectory(app_dir);
  ChangeDirectory("..");

  AddNewAsset("GameAtlas", "assets/1945_atlas.bmp", TEXTURE);
  AddNewAsset("GameIcon", "assets/ui/life.png", IMAGE);
  AddNewAsset("UIBottomBar", "assets/ui/bottom.png", TEXTURE);
}

int main(void) {
  srand(time(NULL));

  char title[100];

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Loading...");
  SetTargetFPS(TARGET_FRAMERATE);

  LoadAllAssets();

  SetWindowIcon(*GetAssetFromName("GameIcon", Image));
  InitAudioDevice();

  /*ECS*/
  size_t worldIndex = CreateWorld();

  //////////////////////////////////
  // Init Systems
  RegisterSystem(InitBg, "INIT");
  // RegisterSystem(InitPlayer, "INIT");

  // // Update Systems
  // RegisterSystem(UpdateBg, "UPDATE");
  // RegisterSystem(UpdatePlayer, "UPDATE");

  // Draw Systems
  RegisterSystem(DrawBg, "DRAW");
  // RegisterSystem(DrawIslandBg, "DRAW");
  // RegisterSystem(DrawPlayer, "DRAW");

  // UI Systems
  RegisterSystem(DrawUi, "UI");
  //////////////////////////////////

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
      RunSystems(worldIndex, "UI");
    }
    EndDrawing();
  }

  FreeAssets();
  DestroyWorld(worldIndex);
  DestroyECS();
  CloseWindow();

  return 0;
}
