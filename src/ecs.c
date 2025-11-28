#include "ecs.h"

#include "aiv_vector.h"
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>

static aiv_vector_t systems = {NULL, 0, 0};

static aiv_vector_t worlds = {NULL, 0, 0};

void DestroyECS() {
  aiv_vector_destroy(&systems);
  aiv_vector_destroy(&worlds);
}

void RegisterSystem(systemFn systemFn, char *tag) {
  if (systems.capacity == 0) {
    systems = aiv_vector_new();
  }

  system_t *system = malloc(sizeof(system_t));
  system->fn = systemFn;
  system->tag = tag;

  aiv_vector_add(&systems, system);
}

void RunSystems(size_t worldIndex, char *tag) {
  world_t *world = worlds.items[worldIndex];
  for (int i = 0; i < systems.count; i++) {
    if (strcmp(((system_t *)systems.items[i])->tag, tag) == 0) {
      ((system_t *)systems.items[i])->fn(world);
    }
  }
}

size_t CreateWorld() {
  if (worlds.capacity == 0) {
    worlds = aiv_vector_new();
  }

  world_t *world = malloc(sizeof(world_t));
  world->components = aiv_vector_new();
  world->entities = aiv_vector_new();

  aiv_vector_add(&worlds, world);

  return worlds.count - 1;
}

void DestroyWorld(size_t worldIndex) {
  world_t *world = worlds.items[worldIndex];

  for (int i = 0; i < world->entities.count; i++) {
    entity_t *entity = world->entities.items[i];
    aiv_vector_destroy(&entity->components);
    free(entity);
  }
  aiv_vector_destroy(&world->entities);

  for (int i = 0; i < world->components.count; i++) {
    component_t *component = world->components.items[i];
    free(component->item);
    free(component);
  }
  aiv_vector_destroy(&world->components);

  free(world);
}

entity_t *CreateEntity(world_t *world) {
  static int entity_index = 0;

  entity_t *new_entity = malloc(sizeof(entity_t));

  new_entity->id = ++entity_index;
  new_entity->world = world;
  new_entity->components = aiv_vector_new();

  aiv_vector_add(&world->entities, new_entity);

  return new_entity;
}

component_t *__addComponent(entity_t *entity, void *item, const char *type) {
  component_t *component = malloc(sizeof(component_t));
  component->componentType = type;
  component->entity = entity;
  component->item = item;
  aiv_vector_add(&entity->components, component);
  aiv_vector_add(&entity->world->components, component);

  return component;
}

component_t *__getComponentOfTypeFromEntity(entity_t *entity, char *type) {
  for (int i = 0; entity->components.count; i++) {
    component_t *c = (component_t *)entity->components.items[i];

    if (strcmp(c->componentType, type) == 0) {
      return c;
    }
  }
  return NULL;
}

component_t *__getComponentOfType(world_t *world, char *type) {
  for (int i = 0; world->components.count; i++) {
    component_t *c = (component_t *)world->components.items[i];

    if (strcmp(c->componentType, type) == 0) {
      return c;
    }
  }
  return NULL;
}

aiv_vector_t __getComponentsOfType(world_t *world, char *type) {
  aiv_vector_t search = aiv_vector_new();

  for (int i = 0; world->components.count; i++) {
    component_t *c = (component_t *)world->components.items[i];

    if (strcmp(c->componentType, type) == 0) {
      aiv_vector_add(&search, c);
    }
  }
  return search;
}

aiv_vector_t __getEntitiesWithTypes(world_t *world, size_t count,
                                    const char *types[]) {
  aiv_vector_t result = aiv_vector_new();

  for (size_t entityIndex = 0; entityIndex < world->entities.count; entityIndex++) {

    entity_t *entity = world->entities.items[entityIndex];
    int components_count = 0;

    for (size_t i = 0; i < entity->components.count; i++) {

      component_t *c = entity->components.items[i];

      for (size_t t = 0; t < count; t++) {
        if (strcmp(c->componentType, types[t]) == 0) {
          components_count++;
        }
      }
    }

    if (components_count == count) {
      aiv_vector_add(&result, entity);
    }
  }

  return result;
}
