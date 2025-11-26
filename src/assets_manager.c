#include <stdlib.h>
#include <types.h>

#include "raylib.h"

static GameAsset* first;

static void* __allocate(const bool result, const char* type, const char* path, const void* src, int size) {
    if (!result) {
        TraceLog(LOG_ERROR, "%s at %s not found", type, path);
        exit(-1);
    }
    void* asset = malloc(size);
    memcpy_s(asset, size, src, size);

    return asset;
}

void AddNewAsset(const char* name, const char* path, const AssetType type) {
    void* asset;
    switch (type) {
        case TEXTURE:
            Texture2D texture = LoadTexture(path);
            asset = __allocate(texture.id, "Texture", path, &texture, sizeof(Texture2D));
            break;
        case IMAGE:
            Image img = LoadImage(path);
            asset = __allocate(img.data, "Image", path, &img, sizeof(Image));
            break;
        default:
            TraceLog(LOG_ERROR, "ASSET LOADER FOR TYPE %d NOT IMPLEMENTED", type);
            exit(-1);
            break;
    }

    GameAsset* current = first;

    if (!current) {
        first = calloc(1, sizeof(GameAsset));
        current = first;
    } else {
        while (current->next) {
            current = current->next;
        }

        current->next = calloc(1, sizeof(GameAsset));
        current = current->next;
    }

    current->name = _strdup(name);
    current->asset = asset;
    current->type = type;
    current->next = NULL;
}

void* GetAssetFromName(const char* name) {
    GameAsset* current = first;

    while (current != NULL) {
        if (strcmp(name, current->name) == 0) {
            return current->asset;
        }

        current = current->next;
    }

    return NULL;
}

void FreeAssets() {
    GameAsset* current = first;

    while (current != NULL) {
        GameAsset* next = current->next;

        // Raylib
        switch (current->type) {
            case TEXTURE:
                UnloadTexture(*(Texture2D*)current->asset);
                break;
            case IMAGE:
                UnloadImage(*(Image*)current->asset);
                break;
            default:
                TraceLog(LOG_ERROR, "ASSET LOADER FOR TYPE %d NOT IMPLEMENTED", current->type);
                exit(-1);
            break;
        }

        free(current->asset);
        free(current->name);
        free(current);

        current = next;
    }

    first = NULL;
}
