#include <corecrt_search.h>
#include <stdlib.h>
#include <types.h>
#include <settings.h>

#include "raylib.h"

static GameAsset *first = NULL;

static void *__allocate(const bool result, const char *type, const char *path, const void *src, int size) {
  if (!result) {
    TraceLog(LOG_ERROR, "%s at %s not found", type, path);
    exit(-1);
  }
  void *asset = malloc(size);
  memcpy_s(asset, size, src, size);

  return asset;
}

void AddAsset(const char *name, void *asset, const AssetType type) {
  GameAsset *current = calloc(1, sizeof(GameAsset));

  current->name = _strdup(name);
  current->asset = asset;
  current->type = type;
  current->next = first;

  first = current;
}

void AddNewAsset(const char *name, const char *path, const AssetType type) {
  void *asset;
  switch (type) {
  case TEXTURE:
    Image imgTmp = LoadImage(path);
    ImageFormat(&imgTmp, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    ImageColorReplace(&imgTmp, KEY_COLOR, (Color){0,0,0, 0});
    Texture2D texture = LoadTextureFromImage(imgTmp);
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

  AddAsset(name, asset, type);
}

void *__getAssetFromName(const char *name) {
  GameAsset *current = first;

  while (current != NULL) {
    if (strcmp(name, current->name) == 0) {
      return current->asset;
    }

    current = current->next;
  }

  return NULL;
}

void FreeAssets() {
  GameAsset *current = first;

  while (current != NULL) {
    GameAsset *next = current->next;

    // Raylib
    switch (current->type) {
    case TEXTURE:
      UnloadTexture(*(Texture2D *)current->asset);
      break;
    case IMAGE:
      UnloadImage(*(Image *)current->asset);
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
