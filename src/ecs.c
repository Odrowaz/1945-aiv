#include "ecs.h"

#include "aiv_vector.h"
#include <corecrt_search.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>

static aiv_vector_t systems = {NULL, 0, 0};
static aiv_vector_t worlds = {NULL, 0, 0};
static aiv_vector_t search = {NULL, 0, 0};

void DestroyECS() {
  aiv_vector_destroy(&systems);
  aiv_vector_destroy(&worlds);
}

void RegisterSystem(SystemFn systemFn, const char *tag) {
  if (systems.capacity == 0) {
    systems = aiv_vector_new();
  }

  System_t *system = malloc(sizeof(System_t));
  system->fn = systemFn;
  system->tag = tag;

  aiv_vector_add(&systems, system);
}

void RunSystems(size_t worldIndex, const char *tag) {
  World_t *world = worlds.items[worldIndex];
  for (int i = 0; i < systems.count; i++) {
    if (strcmp(((System_t *)systems.items[i])->tag, tag) == 0) {
      ((System_t *)systems.items[i])->fn(world);
    }
  }
}

size_t CreateWorld() {
  if (worlds.capacity == 0) {
    worlds = aiv_vector_new();
  }

  World_t *world = malloc(sizeof(World_t));
  world->components = aiv_vector_new();
  world->entities = aiv_vector_new();

  aiv_vector_add(&worlds, world);

  return worlds.count - 1;
}

void DestroyWorld(size_t worldIndex) {
  World_t *world = worlds.items[worldIndex];

  for (int i = 0; i < world->entities.count; i++) {
    Entity_t *entity = world->entities.items[i];
    aiv_vector_destroy(&entity->components);
    free(entity);
  }
  aiv_vector_destroy(&world->entities);

  for (int i = 0; i < world->components.count; i++) {
    Component_t *component = world->components.items[i];
    if (component->item != NULL) {
      free(component->item);
    }
    free(component);
  }
  aiv_vector_destroy(&world->components);

  free(world);
}

Entity_t *CreateEntity(World_t *world) {
  static int entity_index = 0;

  Entity_t *new_entity = malloc(sizeof(Entity_t));

  new_entity->id = ++entity_index;
  new_entity->world = world;
  new_entity->components = aiv_vector_new();

  aiv_vector_add(&world->entities, new_entity);

  return new_entity;
}

Component_t *__addComponent(Entity_t *entity, void *item, const char *type) {
  Component_t *component = malloc(sizeof(Component_t));
  component->componentType = type;
  component->entity = entity;
  component->item = item;
  aiv_vector_add(&entity->components, component);
  aiv_vector_add(&entity->world->components, component);

  return component;
}

Component_t *__getComponentOfTypeFromEntity(Entity_t *entity, const char *type) {
  for (int i = 0; i < entity->components.count; i++) {
    Component_t *c = (Component_t *)entity->components.items[i];

    if (strcmp(c->componentType, type) == 0) {
      return c;
    }
  }
  return NULL;
}

Component_t *__getComponentOfType(World_t *world, const char *type) {
  for (int i = 0; i < world->components.count; i++) {
    Component_t *c = (Component_t *)world->components.items[i];

    if (strcmp(c->componentType, type) == 0) {
      return c;
    }
  }
  return NULL;
}

aiv_vector_t __getComponentsOfType(World_t *world, const char *type) {
  if (search.capacity == 0) {
    search = aiv_vector_new();
  }

  aiv_vector_clear(&search);

  for (int i = 0; i < world->components.count; i++) {
    Component_t *c = (Component_t *)world->components.items[i];

    if (strcmp(c->componentType, type) == 0) {
      aiv_vector_add(&search, c);
    }
  }
  return search;
}

aiv_vector_t __getEntitiesWithTypes(World_t *world, size_t count,
                                    const char *types[]) {
  if (search.capacity == 0) {
    search = aiv_vector_new();
  }

  aiv_vector_clear(&search);

  for (size_t entityIndex = 0; entityIndex < world->entities.count; entityIndex++) {
    Entity_t *entity = world->entities.items[entityIndex];
    bool foundType[count];
    memset(foundType, 0, sizeof(foundType));

    for (size_t i = 0; i < entity->components.count; i++) {
      Component_t *c = entity->components.items[i];
      for (size_t t = 0; t < count; t++) {
        if (!foundType[t] && strcmp(c->componentType, types[t]) == 0) {
          foundType[t] = true;
          break;
        }
      }
    }

    bool allFound = true;
    for (size_t t = 0; t < count; t++) {
      if (!foundType[t]) {
        allFound = false;
        break;
      }
    }

    if (allFound) {
      aiv_vector_add(&search, entity);
    }
  }

  return search;
}

Entity_t *__getEntityWithTypes(World_t *world, size_t count, const char *types[]) {
  for (size_t entityIndex = 0; entityIndex < world->entities.count; entityIndex++) {
    Entity_t *entity = world->entities.items[entityIndex];
    bool foundType[count];
    memset(foundType, 0, sizeof(foundType));

    for (size_t i = 0; i < entity->components.count; i++) {
      Component_t *c = entity->components.items[i];
      for (size_t t = 0; t < count; t++) {
        if (!foundType[t] && strcmp(c->componentType, types[t]) == 0) {
          foundType[t] = true;
          break;
        }
      }
    }

    bool allFound = true;
    for (size_t t = 0; t < count; t++) {
      if (!foundType[t]) {
        allFound = false;
        break;
      }
    }

    if (allFound) {
      return entity;
    }
  }

  return NULL;
}
