#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "ecs.h"
#include "types.h"

void InitExplosions(World_t *world);
void UpdateExplosion(World_t *world);
void EnableExplosion(World_t *world, const Transform2D_t *parentTransform, bool big);

#endif