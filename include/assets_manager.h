#ifndef ASSETS_MANAGER_H
#define ASSETS_MANAGER_H

#include "types.h"

GameAsset* AddNewAsset(const char* name, const char* path, const AssetType type);
void* GetAssetFromName(const char* name);
void FreeAssets();
#endif // ASSETS_MANAGER_H