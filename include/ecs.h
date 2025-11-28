#ifndef ECS_H
#define ECS_H

#include "aiv_vector.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct world {
  aiv_vector_t entities;
  aiv_vector_t components;
} world_t;

typedef struct entity {
  world_t *world;
  size_t id;
  aiv_vector_t components;
} entity_t;

typedef struct component {
  entity_t *entity;
  const char *componentType;
  void *item;
} component_t;

typedef void (*systemFn)(world_t *world);

typedef struct system {
  systemFn fn;
  const char *tag;
} system_t;

#define STR(item) #item

#define DeclareComponent(type)                                                      \
  static component_t *__addComponentCpy##type(entity_t *entity, const type *item) { \
    void *item_cpy = malloc(sizeof(type));                                          \
    memcpy(item_cpy, item, sizeof(type));                                           \
    return __addComponent(entity, item_cpy, #type);                                 \
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

aiv_vector_t __getComponentsOfType(world_t *world, const char *type);
component_t *__getComponentOfType(world_t *world, const char *type);
component_t *__getComponentOfTypeFromEntity(entity_t *entity, const char *type);

aiv_vector_t __getEntitiesWithTypes(world_t *world, size_t count,

                                    const char *types[]);

void RegisterSystem(systemFn systemFn, const char *tag);

void RunSystems(size_t worldIndex, const char *tag);

size_t CreateWorld();
void DestroyWorld(size_t worldIndex);

entity_t *CreateEntity(world_t *world);
component_t *__addComponent(entity_t *entity, void *item, const char *type);

void DestroyECS();

#endif
