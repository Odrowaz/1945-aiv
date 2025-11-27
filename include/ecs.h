#ifndef ECS_H
#define ECS_H

#include "aiv_vector.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define STR(item) #item

typedef struct entity {
  size_t id;
} entity_t;

typedef struct component {
  size_t entity_id;
  char *componentType;
  void *item;
} component_t;

typedef struct world {
  aiv_vector_t entities;
  aiv_vector_t components;
} world_t;

typedef void (*systemFn)(world_t *world);

typedef struct system {
  systemFn fn;
  char *tag;
} system_t;

#define DeclareComponent(type)                                                 \
  static void __addComponent##type(world_t *world, const type *item,                  \
                            entity_t *entity) {                                \
    component_t *component = malloc(sizeof(component_t));                      \
    component->componentType = #type;                                          \
    component->entity_id = entity->id;                                         \
    component->item = malloc(sizeof(type));                                    \
    memcpy(component->item, item, sizeof(type));                               \
    aiv_vector_add(&world->components, component);                             \
  }

#define AddComponent(world, component, type, entity)                           \
  __addComponent##type(world, component, entity)

#define GetComponentOfType(world, type) __getComponentOfType(world, #type)

#define GetComponentsOfType(world, type) __getComponentsOfType(world, #type)

#define GetEntitiesWithTypes(world, ...)                                       \
  __getEntitiesWithTypes(                                                      \
      world, sizeof((const char *[]){__VA_ARGS__}) / sizeof(void *),           \
      (const char *[]){__VA_ARGS__})

world_t __getComponentsOfType(world_t *world, char *type);

component_t *__getComponentOfType(world_t *world, char *type);

world_t __getEntitiesWithTypes(world_t *world, size_t count,
                               const char *types[]);

void RegisterSystem(systemFn systemFn, char *tag);

void RunSystems(world_t *world, char *tag);

world_t CreateWorld();

entity_t CreateEntity(world_t *world);

world_t GetComponentsFromEntity(world_t *world, entity_t *entity);

#endif
