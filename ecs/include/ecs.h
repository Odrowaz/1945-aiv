#ifndef ECS_H
#define ECS_H

#include "aiv_vector.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#if defined(_WIN32)
#define ECS_API __declspec(dllexport)
#else
#define ECS_API
#endif

typedef struct World {
  aiv_vector_t entities;
  aiv_vector_t components;
} World_t;

typedef struct Entity {
  World_t *world;
  size_t id;
  bool enabled;
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

#define DeclareComponent(type)                                                                                \
  static Component_t *__addComponentCpy##type(Entity_t *entity, const type item) {                            \
    void *item_cpy = malloc(sizeof(type));                                                                    \
    memcpy(item_cpy, &item, sizeof(type));                                                                    \
    return __addComponent(entity, item_cpy, #type);                                                           \
  }                                                                                                           \
  static Component_t *__addComponentCpyTag##type(Entity_t *entity, const type item, const char *typeString) { \
    void *item_cpy = malloc(sizeof(type));                                                                    \
    memcpy(item_cpy, &item, sizeof(type));                                                                    \
    return __addComponent(entity, item_cpy, typeString);                                                      \
  }

#define AddComponent(entity, component, type) __addComponent(entity, component, #type)

#define AddComponentCpyTag(entity, component, type, tag) \
  __addComponentCpyTag##type(entity, component, #tag)

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

ECS_API aiv_vector_t __getComponentsOfType(World_t *world, const char *type);
ECS_API Component_t *__getComponentOfType(World_t *world, const char *type);
ECS_API Component_t *__getComponentOfTypeFromEntity(Entity_t *entity, const char *type);

ECS_API aiv_vector_t __getEntitiesWithTypes(World_t *world, size_t count, const char *types[]);
ECS_API Entity_t *__getEntityWithTypes(World_t *world, size_t count, const char *types[]);

ECS_API void RegisterSystem(SystemFn systemFn, const char *tag);

ECS_API void RunSystems(size_t worldIndex, const char *tag);

ECS_API size_t CreateWorld();
ECS_API void DestroyWorld(size_t worldIndex);

ECS_API Entity_t *CreateEntity(World_t *world);
ECS_API Component_t *__addComponent(Entity_t *entity, void *item, const char *type);

ECS_API void DestroyECS();

DeclareComponent(int)
DeclareComponent(bool)

#endif
