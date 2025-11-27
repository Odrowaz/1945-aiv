#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "ecs.h"

typedef enum AssetType {
    TEXTURE,
    MUSIC,
    SOUND,
    IMAGE
} AssetType;

typedef struct GameAsset {
    struct GameAsset* next;
    char* name;
    void* asset;
    AssetType type;
} GameAsset;

typedef struct ScrollingBG {
    size_t offset_y;
    Texture2D* atlas;
} ScrollingBG;
DeclareComponent(ScrollingBG);

typedef struct Timer {
    double startingTime;
    bool started;
} Timer;
DeclareComponent(Timer);

#endif  // TYPES_H