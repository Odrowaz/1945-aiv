#ifndef ECS_H
#define ECS_H

#include "aiv_vector.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct World {
  aiv_vector_t entities;
  aiv_vector_t components;
} World_t;

typedef struct Entity {
  World_t *world;
  size_t id;
  aiv_vector_t components;
} Entity_t;

typedef struct Component {
  Entity_t *entity;
  const char *componentType;
  void *item;
} Component_t;

typedef void (*SystemFn)(World_t *world);

typedef struct System {
  SystemFn fn;
  const char *tag;
} System_t;

#define STR(item) #item

#define DeclareComponent(type)                                                     \
  static Component_t *__addComponentCpy##type(Entity_t *entity, const type item) { \
    void *item_cpy = malloc(sizeof(type));                                         \
    memcpy(item_cpy, &item, sizeof(type));                                         \
    return __addComponent(entity, item_cpy, #type);                                \
  }

#define AddComponent(entity, component, type) __addComponent(entity, component, #type)

#define AddComponentCpy(entity, component, type) \
  __addComponentCpy##type(entity, component)

#define GetComponentFromEntity(entity, type) \
  __getComponentOfTypeFromEntity(entity, #type)

#define GetComponent(world, type) __getComponentOfType(world, #type)

#define GetComponents(world, type) __getComponentsOfType(world, #type)

#define GetEntitiesWithTypes(world, ...)                             \
  __getEntitiesWithTypes(                                            \
      world, sizeof((const char *[]){__VA_ARGS__}) / sizeof(void *), \
      (const char *[]){__VA_ARGS__})

#define GetEntityWithTypes(world, ...)                               \
  __getEntityWithTypes(                                              \
      world, sizeof((const char *[]){__VA_ARGS__}) / sizeof(void *), \
      (const char *[]){__VA_ARGS__})

aiv_vector_t __getComponentsOfType(World_t *world, const char *type);
Component_t *__getComponentOfType(World_t *world, const char *type);
Component_t *__getComponentOfTypeFromEntity(Entity_t *entity, const char *type);

aiv_vector_t __getEntitiesWithTypes(World_t *world, size_t count, const char *types[]);
Entity_t *__getEntityWithTypes(World_t *world, size_t count, const char *types[]);

void RegisterSystem(SystemFn systemFn, const char *tag);

void RunSystems(size_t worldIndex, const char *tag);

size_t CreateWorld();
void DestroyWorld(size_t worldIndex);

Entity_t *CreateEntity(World_t *world);
Component_t *__addComponent(Entity_t *entity, void *item, const char *type);

void DestroyECS();

#endif
