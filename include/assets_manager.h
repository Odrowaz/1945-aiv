#ifndef ASSETS_MANAGER_H
#define ASSETS_MANAGER_H

#include "types.h"

#define GetAssetFromName(name, type) (type*) __getAssetFromName(name)
void* __getAssetFromName(const char* name);
GameAsset* AddNewAsset(const char* name, const char* path, const AssetType type);
void AddAsset(const char* name, const void* asset, const AssetType type);
void FreeAssets();
#endif // ASSETS_MANAGER_H