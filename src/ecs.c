#include "ecs.h"

static aiv_vector_t systems = {NULL, 0, 0};

void RegisterSystem(systemFn systemFn, char *tag) {
  if (systems.capacity == 0) {
    systems = aiv_vector_new();
  }

  system_t *system = malloc(sizeof(system_t));
  system->fn = systemFn;
  system->tag = tag;

  aiv_vector_add(&systems, system);
}

void RunSystems(world_t *world, char *tag) {
  for (int i = 0; i < systems.count; i++) {
    if (strcmp(((system_t *)systems.items[i])->tag, tag) == 0) {
      ((system_t *)systems.items[i])->fn(world);
    }
  }
}

world_t CreateWorld() {
  world_t world = {
      aiv_vector_new(),
      aiv_vector_new(),
  };

  return world;
}

entity_t CreateEntity(world_t *world) {
  static int entity_count = 0;

  entity_t *new_entity = malloc(sizeof(entity_t));

  new_entity->id = entity_count++;

  aiv_vector_add(&world->entities, new_entity);

  return *new_entity;
}

world_t GetComponentsFromEntity(world_t *world, entity_t *entity) {
  world_t search = CreateWorld();

  for (int i = 0; i < world->components.count; i++) {
    component_t *c = world->components.items[i];

    if (c->entity_id == entity->id) {
      aiv_vector_add(&search.components, c);
    }
  }

  return search;
}

component_t *__getComponentOfType(world_t *world, char *type) {
  world_t search = CreateWorld();

  for (int i = 0; world->components.count; i++) {
    component_t *c = (component_t *)world->components.items[i];

    if (strcmp(c->componentType, type) == 0) {
      return c;
    }
  }
  return NULL;
}

world_t __getComponentsOfType(world_t *world, char *type) {
  world_t search = CreateWorld();

  for (int i = 0; world->components.count; i++) {
    component_t *c = (component_t *)world->components.items[i];

    if (strcmp(c->componentType, type) == 0) {
      aiv_vector_add(&search.components, c);
    }
  }
  return search;
}

world_t __getEntitiesWithTypes(world_t *world, size_t count,
                               const char *types[]) {
  world_t result = CreateWorld();

  for (size_t entityIndex = 0; entityIndex < world->entities.count;
       entityIndex++) {
    entity_t *entity = world->entities.items[entityIndex];
    int components_count = 0;
    for (size_t i = 0; i < world->components.count; i++) {

      component_t *c = (component_t *)world->components.items[i];

      if (entity->id != c->entity_id)
        continue;

      for (size_t t = 0; t < count; t++) {
        if (strcmp(c->componentType, types[t]) == 0) {
          components_count++;
        }
      }
    }
    if (components_count == count) {
      aiv_vector_add(&result.entities, entity);
    }
  }

  return result;
}
