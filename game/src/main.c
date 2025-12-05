#include "assets_manager.h"
#include "bg.h"
#include "bullet.h"
#include "ecs.h"
#include "enemy.h"
#include "player.h"
#include "raylib.h"
#include "settings.h"
#include "timers.h"
#include "types.h"
#include "explosion.h"
#include "ui.h"
#include <corecrt_search.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

void LoadAllAssets() {
  const char *app_dir = GetApplicationDirectory();
  ChangeDirectory(app_dir);

  AddNewAsset("GameAtlas", "assets/1945_atlas.bmp", TEXTURE);
  AddNewAsset("GameIcon", "assets/ui/life.png", IMAGE);
  AddNewAsset("UIBottomBar", "assets/ui/bottom.png", TEXTURE);
}

void DrawCollider(World_t *world) {

  aiv_vector_t search = GetEntitiesWithTypes(world, STR(Collider_t), STR(Transform2D_t));

  for (int i = 0; i < search.count; i++) {
    Entity_t *entity = search.items[i];
    if (!entity->enabled) continue;

    Transform2D_t *transform = GetComponentFromEntity(entity, Transform2D_t)->item;
    Collider_t *collider = GetComponentFromEntity(entity, Collider_t)->item;

    DrawRectangleLines(transform->translation.x + collider->size.x, transform->translation.y + collider->size.y, collider->size.width, collider->size.height, RED);
  }
}

void Draw(World_t *world) {

  aiv_vector_t search = GetEntitiesWithTypes(world, STR(Sprite_t), STR(Transform2D_t));

  for (int i = 0; i < search.count; i++) {
    Entity_t *entity = search.items[i];
    if (!entity->enabled) continue;

    Sprite_t *sprite = GetComponentFromEntity(entity, Sprite_t)->item;
    Transform2D_t *transform = GetComponentFromEntity(entity, Transform2D_t)->item;
    Component_t *animationComponent = GetComponentFromEntity(entity, Animation_t);
    Animation_t *animation = NULL;
    int frameIndex = -1;

    if (animationComponent != NULL) {
      animation = animationComponent->item;
      frameIndex = GetAnimationFrame(&animation->timer, animation->frames, animation->frameDuration);
    }

    DrawTexturePro(*sprite->atlas,
                   frameIndex != -1 ? animation->animation[frameIndex] : sprite->tile,
                   (Rectangle){transform->translation.x, transform->translation.y, sprite->size * transform->scale.x, sprite->size * transform->scale.y},
                   (Vector2){sprite->size * 0.5, sprite->size * 0.5}, transform->rotation, WHITE);
  }
}

void Collision(World_t *world) {
  aiv_vector_t search = GetEntitiesWithTypes(world, STR(Collider_t), STR(Transform2D_t));

  for (size_t i = 0; i < search.count; i++) {
    Entity_t *entity1 = search.items[i];
    if (!entity1->enabled) continue;

    Transform2D_t *entity1Transform = GetComponentFromEntity(entity1, Transform2D_t)->item;
    Collider_t *entity1Collider = GetComponentFromEntity(entity1, Collider_t)->item;

    Rectangle entity1Position = {entity1Transform->translation.x + entity1Collider->size.x, entity1Transform->translation.y + entity1Collider->size.y, entity1Collider->size.width, entity1Collider->size.height};

    for (size_t j = i + 1; j < search.count; j++) {
      Entity_t *entity2 = search.items[j];
      if (!entity2->enabled) continue;

      Transform2D_t *entity2Transform = GetComponentFromEntity(entity2, Transform2D_t)->item;
      Collider_t *entity2Collider = GetComponentFromEntity(entity2, Collider_t)->item;

      Rectangle entity2Position = {entity2Transform->translation.x + entity2Collider->size.x, entity2Transform->translation.y + entity2Collider->size.y, entity2Collider->size.width, entity2Collider->size.height};

      if (CheckCollisionRecs(entity1Position, entity2Position)) {

        if (entity1Collider->collisionmask & entity2Collider->layermask) {
          entity1Collider->hit = true;
        }

        if (entity2Collider->collisionmask & entity1Collider->layermask) {
          entity2Collider->hit = true;
        }

      }
    }
  }
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
  RegisterSystem(InitBullets, "INIT");
  RegisterSystem(InitPlayer, "INIT");
  RegisterSystem(InitEnemies, "INIT");
  RegisterSystem(InitExplosions, "INIT");

  // Update Systems
  RegisterSystem(UpdateBg, "UPDATE");
  RegisterSystem(UpdatePlayer, "UPDATE");
  RegisterSystem(UpdateBullet, "UPDATE");
  RegisterSystem(UpdateEnemies, "UPDATE");
  RegisterSystem(UpdateBoss, "UPDATE");
  RegisterSystem(UpdateExplosion, "UPDATE");

  RegisterSystem(Collision, "PHYSICS");

  // Draw Systems
  RegisterSystem(DrawBg, "DRAW");
  RegisterSystem(Draw, "DRAW");
#ifdef DEBUG_BUILD
  RegisterSystem(DrawCollider, "DRAW");
#endif

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

    RunSystems(worldIndex, "PHYSICS");

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
