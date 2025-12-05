#ifndef TYPES_H
#define TYPES_H

#include "ecs.h"
#include "raylib.h"
#include <math.h>
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
  float offset_y;
  Texture2D *atlas;
} ScrollingBG_t;
DeclareComponent(ScrollingBG_t);

typedef struct Timer {
  double startingTime;
  bool started;
} Timer_t;
DeclareComponent(Timer_t);

// typedef struct Bullet {
// } Bullet_t;
// DeclareComponent(Bullet_t);

typedef struct Collider {
  Rectangle size;
  bool hit;
  unsigned int layermask;
  unsigned int collisionmask;
} Collider_t;
DeclareComponent(Collider_t);

typedef struct Animation {
  Timer_t timer;
  float frameDuration;
  int frames;
  const Rectangle *animation;
} Animation_t;
DeclareComponent(Animation_t);

typedef struct Sprite {
  Rectangle tile;
  Texture2D *atlas;
  int size;
} Sprite_t;
DeclareComponent(Sprite_t);

typedef struct Transform2D {
  Vector2 translation;
  float rotation;
  Vector2 scale;
} Transform2D_t;
DeclareComponent(Transform2D_t);

DeclareComponent(Vector2)

#endif // TYPES_H