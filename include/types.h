#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"
#include <stdbool.h>
#include <stddef.h>
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
    int offset_y;
    Texture2D* atlas;
} ScrollingBG;
DeclareComponent(ScrollingBG);

typedef struct ScrollingIslandBG {
    int x;
    int y;
    Texture2D* atlas;
    float rotation;
    Rectangle tile;
    bool enabled;
} ScrollingIslandBG;
DeclareComponent(ScrollingIslandBG);

typedef struct Timer {
    double startingTime;
    bool started;
} Timer;
DeclareComponent(Timer);

#endif  // TYPES_H