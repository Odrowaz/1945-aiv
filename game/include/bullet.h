#ifndef BULLET_H
#define BULLET_H

#include "ecs.h"
#include "types.h"

void InitBullets(World_t *world);
void ShootBullet(World_t *world, Transform2D_t *transform, unsigned int layermask, unsigned int collisionmask, float rotation, int bullets, int startIndex, int offset_y);
void UpdateBullet(World_t *world);

#endif