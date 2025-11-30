#include "assets_manager.h"
#include "ecs.h"
#include "raylib.h"
#include "settings.h"

void DrawUi(World_t *world) {

  DrawTexturePro(*GetAssetFromName("UIBottomBar", Texture2D),
                 ((Rectangle){0, 0, 640, 76}),
                 ((Rectangle){0, SCREEN_HEIGHT - UI_BAR_HEIGHT, SCREEN_WIDTH, UI_BAR_HEIGHT}),
                 (Vector2){0, 0}, 0, WHITE);
}