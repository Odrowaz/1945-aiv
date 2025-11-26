#ifndef TYPES_H
#define TYPES_H

#include <string.h>

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

#endif  // TYPES_H