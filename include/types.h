#ifndef TYPES_H
#define TYPES_H

#include "ecs.h"
#include "raylib.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum AssetType {
  TEXTURE,
  MUSIC,
  SOUND,
  IMAGE
} AssetType;

typedef struct GameAsset {
  struct GameAsset *next;
  char *name;
  void *asset;
  AssetType type;
} GameAsset_t;

typedef struct ScrollingBG {
  int offset_y;
  Texture2D *atlas;
} ScrollingBG_t;
DeclareComponent(ScrollingBG_t);

typedef struct ScrollingIslandBG {
  Texture2D *atlas;
  Rectangle tile;
  bool enabled;
} ScrollingIslandBG_t;
DeclareComponent(ScrollingIslandBG_t);

typedef struct Timer {
  double startingTime;
  bool started;
} Timer_t;
DeclareComponent(Timer_t);

typedef struct Transform2D {
  Vector2 translation;
  float rotation;
  Vector2 scale;
} Transform2D_t;
DeclareComponent(Transform2D_t);

#endif // TYPES_H