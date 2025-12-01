#include "ecs.h"

#include "aiv_vector.h"
#include <corecrt_search.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>

static aiv_vector_t systems = {NULL, 0, 0};
static aiv_vector_t worlds = {NULL, 0, 0};
static aiv_vector_t entity_search = {NULL, 0, 0};
static aiv_vector_t component_search = {NULL, 0, 0};

void DestroyECS() {
  aiv_vector_destroy(&systems);
  aiv_vector_destroy(&worlds);
  aiv_vector_destroy(&entity_search);
  aiv_vector_destroy(&component_search);
}

void RegisterSystem(SystemFn systemFn, const char *tag) {
  if (systems.capacity == 0) {
    systems = aiv_vector_new(sizeof(System_t));
  }

  System_t system = {systemFn, tag};

  aiv_vector_add(&systems, &system);
}

void RunSystems(size_t worldIndex, const char *tag) {
  World_t *world = aiv_vector_at(&worlds, worldIndex);
  for (int i = 0; i < systems.count; i++) {
    if (strcmp(((System_t *)aiv_vector_at(&systems, i))->tag, tag) == 0) {
      ((System_t *)aiv_vector_at(&systems, i))->fn(world);
    }
  }
}

size_t CreateWorld() {
  if (worlds.capacity == 0) {
    worlds = aiv_vector_new(sizeof(World_t));
  }

  World_t world = {aiv_vector_new(sizeof(Entity_t)),
                   aiv_vector_new(sizeof(Component_t))};

  aiv_vector_add(&worlds, &world);

  return worlds.count - 1;
}

void DestroyWorld(size_t worldIndex) {
  World_t *world = aiv_vector_at(&worlds, worldIndex);

  for (int i = 0; i < world->entities.count; i++) {
    Entity_t *entity = aiv_vector_at(&world->entities, i);
    aiv_vector_destroy(&entity->components);
    free(entity);
  }
  aiv_vector_destroy(&world->entities);

  for (int i = 0; i < world->components.count; i++) {
    Component_t *component = aiv_vector_at(&world->components, i);
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

  Entity_t new_entity = {
      world, ++entity_index, aiv_vector_new(sizeof(Component_t))};

  aiv_vector_add(&world->entities, &new_entity);

  return aiv_vector_at(&world->entities, world->entities.count - 1);
}

Component_t *__addComponent(Entity_t *entity, void *item, const char *type) {
  Component_t component = {
      entity, type, item};

  aiv_vector_add(&entity->components, &component);
  aiv_vector_add(&entity->world->components, &component);

  return aiv_vector_at(&entity->components, entity->components.count - 1);
}

Component_t *__getComponentOfTypeFromEntity(Entity_t *entity, const char *type) {
  for (int i = 0; i < entity->components.count; i++) {
    Component_t *c = aiv_vector_at(&entity->components, i);

    if (strcmp(c->componentType, type) == 0) {
      return c;
    }
  }
  return NULL;
}

Component_t *__getComponentOfType(World_t *world, const char *type) {
  for (int i = 0; i < world->components.count; i++) {
    Component_t *c = aiv_vector_at(&world->components, i);

    if (strcmp(c->componentType, type) == 0) {
      return c;
    }
  }
  return NULL;
}

aiv_vector_t __getComponentsOfType(World_t *world, const char *type) {
  if (component_search.capacity == 0) {
    component_search = aiv_vector_new(sizeof(Component_t));
  }

  aiv_vector_clear(&component_search);

  for (int i = 0; i < world->components.count; i++) {
    Component_t *c = aiv_vector_at(&world->components, i);

    if (strcmp(c->componentType, type) == 0) {
      aiv_vector_add(&component_search, c);
    }
  }
  return component_search;
}

aiv_vector_t __getEntitiesWithTypes(World_t *world, size_t count,
                                    const char *types[]) {
  if (entity_search.capacity == 0) {
    entity_search = aiv_vector_new(sizeof(Entity_t));
  }

  aiv_vector_clear(&entity_search);

  for (size_t entityIndex = 0; entityIndex < world->entities.count; entityIndex++) {
    Entity_t *entity = aiv_vector_at(&world->entities, entityIndex);
    bool foundType[count];
    memset(foundType, 0, sizeof(foundType));

    for (size_t i = 0; i < entity->components.count; i++) {
      Component_t *c = aiv_vector_at(&entity->components, i);
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
      aiv_vector_add(&entity_search, entity);
    }
  }

  return entity_search;
}

Entity_t *__getEntityWithTypes(World_t *world, size_t count, const char *types[]) {
  for (size_t entityIndex = 0; entityIndex < world->entities.count; entityIndex++) {
    Entity_t *entity = aiv_vector_at(&world->entities, entityIndex);
    bool foundType[count];
    memset(foundType, 0, sizeof(foundType));

    for (size_t i = 0; i < entity->components.count; i++) {
      Component_t *c = aiv_vector_at(&entity->components, i);
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
