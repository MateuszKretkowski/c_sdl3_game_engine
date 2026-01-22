#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gameObject.h"
#include "component.h"
#include <libs/cJSON.h>

GameObject *instantiate_gameObject(char* id) {
    GameObject *gameObject = malloc(sizeof(GameObject));
    if (!gameObject) {
        fprintf(stderr, "instantiate_gameObject: failed to allocate memory for GameObject\n");
        return NULL;
    }
    gameObject->id = malloc(strlen(id) * sizeof(char) + sizeof(char));
    if (!gameObject->id) {
        fprintf(stderr, "instantiate_gameObject: failed to allocate memory for id: %s\n", id);
        free(gameObject);
        return NULL;
    }
    gameObject->name = NULL;
    

    strcpy(gameObject->id, id);
    gameObject->components = malloc(sizeof(Component*) * 8);
    if (!gameObject->components) {
        fprintf(stderr, "instantiate_gameObject: failed to allocate memory for components array\n");
        free(gameObject->id);
        free(gameObject);
        return NULL;
    }
    gameObject->components_capacity = 8;
    gameObject->components_length = 0;

    gameObject->gameObjects = malloc(sizeof(GameObject*) * 8);
    if (!gameObject->components) {
        fprintf(stderr, "instantiate_gameObject: failed to allocate memory for gameObjects array\n");
        free(gameObject->id);
        free(gameObject);
        return NULL;
    }
    gameObject->gameObjects_capacity = 8;
    gameObject->gameObjects_length = 0;

    return gameObject;
}

void free_gameObject(GameObject *gameObject) {
    if (!gameObject) {
        fprintf(stderr, "free_gameObject: gameObject is NULL\n");
        return;
    }
    for (int i=0; i<gameObject->components_length; i++) {
        local_destroy_component(gameObject->components[i]);
    }
    free(gameObject->components);
    free(gameObject->id);
    free(gameObject->name);
    free(gameObject);
}

void add_component(GameObject *gameObject, Component *component) {
    // Walidacja parametrów
    if (!gameObject) {
        fprintf(stderr, "add_component: ERROR - gameObject is NULL\n");
        return;
    }
    
    if (!component) {
        fprintf(stderr, "add_component: ERROR - component is NULL\n");
        return;
    }
    
    if (gameObject->components == NULL) {
        fprintf(stderr, "add_component: ERROR - gameObject->components array is NULL (gameObject: %p)\n", 
                (void*)gameObject);
        return;
    }
    
    // Sprawdzenie poprawności capacity
    if (gameObject->components_capacity == 0) {
        fprintf(stderr, "add_component: ERROR - components_capacity is 0 (gameObject: %p)\n", 
                (void*)gameObject);
        return;
    }
    
    // Sprawdzenie czy length nie przekracza capacity (błąd stanu)
    if (gameObject->components_length > gameObject->components_capacity) {
        fprintf(stderr, "add_component: ERROR - components_length (%d) exceeds capacity (%d) (gameObject: %p)\n",
                gameObject->components_length, gameObject->components_capacity, (void*)gameObject);
        return;
    }
    
    // Sprawdzenie przepełnienia przy mnożeniu capacity * 2
    if (gameObject->components_length == gameObject->components_capacity) {
        if (gameObject->components_capacity > SIZE_MAX / 2 / sizeof(Component*)) {
            fprintf(stderr, "add_component: ERROR - capacity overflow would occur (current capacity: %d)\n",
                    gameObject->components_capacity);
            return;
        }
        
        size_t new_capacity = gameObject->components_capacity * 2;
        Component **new_components = realloc(gameObject->components, sizeof(Component*) * new_capacity);
        
        if (!new_components) {
            fprintf(stderr, "add_component: ERROR - failed to reallocate components array "
                           "(tried to allocate %zu bytes for capacity %d)\n",
                    sizeof(Component*) * new_capacity, new_capacity);
            return;
        }
        
        gameObject->components = new_components;
        gameObject->components_capacity = new_capacity;
    }

    // Dodanie komponentu
    component->gameObject = gameObject;
    gameObject->components[gameObject->components_length] = component;
    gameObject->components_length++;
}

void remove_component_by_id(GameObject *gameObject, const char *id) {
    if (!gameObject || !id) {
        const char *safe_id = id ? id : "NULL_ID"; 
        fprintf(stderr, "remove_component_by_id: Invalid parameters. GameObject: %p, ID: %s\n", (void*)gameObject, safe_id);
        return;
    }
    if (strlen(id) == 0) {
        fprintf(stderr, "remove_component_by_id: Component ID cannot be an empty string.\n");
        return;
    }
    if (gameObject->components == NULL) {
        fprintf(stderr, "remove_component_by_id: Component list is NULL on GameObject '%s'. ID: %s\n",
                gameObject->name ? gameObject->name : "NULL", id);
        return;
    }
    if (gameObject->components_length <= 0) {
        fprintf(stderr, "remove_component_by_id: Component list is empty on GameObject '%s'. ID: %s\n",
                gameObject->name ? gameObject->name : "NULL", id);
        return;
    }
    for (int i = 0; i < gameObject->components_length; i++) {
        
        if (gameObject->components[i] == NULL) {
            fprintf(stderr, "remove_component_by_id: Component at index %d is NULL. Skipping.\n", i);
            continue;
        }
        if (gameObject->components[i]->name == NULL) {
             fprintf(stderr, "remove_component_by_id: Component at index %d has NULL name. Skipping.\n", i);
             continue; 
        }
        if (strcmp(gameObject->components[i]->id, id) == 0) {
            local_destroy_component(gameObject->components[i]);
            if (i == gameObject->components_length - 1) {
                gameObject->components[i] = NULL;
            } else {
                gameObject->components[i] = gameObject->components[gameObject->components_length - 1];
            }

            gameObject->components_length--;
            return;
        }
    }
    
    // Nie znaleziono komponentu - wypisz wszystkie dostępne komponenty
    fprintf(stderr, "remove_component_by_id: Could not find component with ID/Name '%s' on GameObject '%s'.\n",
            id, gameObject->name ? gameObject->name : "NULL");
    fprintf(stderr, "Available components (%d total):\n", gameObject->components_length);
    
    for (int i = 0; i < gameObject->components_length; i++) {
        if (gameObject->components[i] == NULL) {
            fprintf(stderr, "  [%d] NULL component\n", i);
        } else if (gameObject->components[i]->name == NULL) {
            fprintf(stderr, "  [%d] Component at %s (NULL name)\n", i, (void*)gameObject->components[i]->id);
        } else {
            fprintf(stderr, "  [%d] '%s' (component at %p)\n", i, gameObject->components[i]->id, (void*)gameObject->components[i]);
        }
    }
}

void remove_component(GameObject *gameObject, Component *component) {
    if (!gameObject || !component || gameObject->components == NULL || !component || strcmp(component->id, "transform_component") == 0) {
        fprintf(stderr, "remove_component: invalid parameters - gameObject: %p, component: %p\n", (void*)gameObject, (void*)component);
        return;
    }
    Component *test = get_component_base(gameObject, component->id);
    printf("chcemy wyrzucic: %s", component->id);
    printf("wyrzucamy: %s", test->id);
    if (!test) {
        printf("\ncould not get component base: %s\n", test->id);
        return;
    }
        printf("\nCOULD get component base\n");
    
    for (int i=0; i<gameObject->components_length; i++) {
        if (strcmp(gameObject->components[i]->name, component->name) == 0) {
            printf("\ngameobject.components[i]: %s", *gameObject->components[i]->id);
            local_destroy_component(gameObject->components[i]);
            gameObject->components[i] = gameObject->components[gameObject->components_length-1];
            gameObject->components_length--;
            return;
        }
    }

    fprintf(stderr, "remove_component: could not find component to remove: %s\n", component->name);
}

Component *get_component_base(GameObject *gameObject, const char *id) {
    if (!gameObject || !id) {
        fprintf(stderr, "get_component_base: invalid parameters - gameObject: %p, id: %p\n", (void*)gameObject, (void*)id);
        return NULL;
    }
    for (int i = 0; i < gameObject->components_length; i++) {
        if (strcmp(gameObject->components[i]->id, id) == 0) {
            return gameObject->components[i];
        }
    }
    return NULL;
}

// Child Actions:
GameObject *get_child(GameObject *parent, int index) {
    if (!parent || index < 0) {
        printf("get_child(): invalid parameters;\n");
    }
    if (parent->gameObjects_length < index) {
        printf("get_child(): index too large;\n");
    }
    return parent->gameObjects[index];
}

GameObject **get_children(GameObject *gameObject) {
    if (!gameObject) {
        printf("get_children(): invalid parameter;\n");
        return NULL;
    }
    if (gameObject->gameObjects_length <= 0) {
        return NULL;
    }
    return gameObject->gameObjects;
}

void add_child(GameObject *parent, GameObject *child) {
    if (!parent || !child) {
        printf("add_child(): invalid parameters\n");
        return;
    }
    if (parent->gameObjects_length == parent->gameObjects_capacity) {
        parent->gameObjects_capacity *= 2;
        parent->gameObjects = realloc(parent->gameObjects, sizeof(GameObject*)*parent->gameObjects_capacity);
    }
    parent->gameObjects[parent->gameObjects_length] = child;
    parent->gameObjects_length++;
}

void remove_child_by_gameObject(GameObject *parent, GameObject *child) {
    if (!parent || !child) {
        printf("remove_child(): invalid parameters\n");
        return;
    }
    for (int i=0; i<parent->gameObjects_length; i++) {
        if (strcmp(parent->gameObjects[i]->id, child->id) == 0) {
            parent->gameObjects[i] = parent->gameObjects[parent->gameObjects_length];
            parent->gameObjects_length--;
            return;
        }
    }
    printf("remove_child(): child not found;\n");
}

void remove_child(GameObject *parent, int index) {
    if (!parent || index < 0) {
        printf("remove_child_by_id(): invalid parameters\n");
        return;
    }
    if (index > parent->gameObjects_length) {
        printf("remove_child_by_id(): index too large;\n");
        return;
    }
    parent->gameObjects[index] = parent->gameObjects[parent->gameObjects_length];
    parent->gameObjects_length--;
}

GameObject *instantiate_prefab(GameObject *prefab) {
    if (!prefab) {
        fprintf(stderr, "instantiate_prefab: ERROR - prefab is NULL\n");
        return NULL;
    }

    GameObject *instance = instantiate_gameObject(prefab->name);
    if (!instance) {
        fprintf(stderr, "instantiate_prefab: ERROR - failed to instantiate gameObject\n");
        return NULL;
    }

    if (prefab->id) {
        free(instance->id);
        instance->id = strdup(prefab->id);
        if (!instance->id) {
            fprintf(stderr, "instantiate_prefab: ERROR - failed to duplicate prefab id\n");
            free_gameObject(instance);
            return NULL;
        }
    }

    if (prefab->name) {
        instance->name = strdup(prefab->name);
        if (!instance->name) {
            fprintf(stderr, "instantiate_prefab: ERROR - failed to duplicate prefab name\n");
            free_gameObject(instance);
            return NULL;
        }
    }

    if (prefab->components_length == 0) {
        fprintf(stderr, "instantiate_prefab: WARNING - prefab has no components\n");
        return instance;
    }

    for (int i = 0; i < prefab->components_length; i++) {
        Component *original = prefab->components[i];

        if (!original) {
            fprintf(stderr, "instantiate_prefab: WARNING - component at index %d is NULL, skipping\n", (int)i);
            continue;
        }

        Component *copy = malloc(original->size);
        if (!copy) {
            fprintf(stderr, "instantiate_prefab: ERROR - failed to allocate %zu bytes for component copy\n", 
                    original->size);
            free_gameObject(instance);
            return NULL;
        }

        memcpy(copy, original, original->size);

        if (original->id) {
            copy->id = strdup(original->id);
            if (!copy->id) {
                fprintf(stderr, "instantiate_prefab: ERROR - failed to duplicate component id\n");
                free(copy);
                free_gameObject(instance);
                return NULL;
            }
        } else {
            copy->id = NULL;
        }

        if (original->name) {
            copy->name = strdup(original->name);
            if (!copy->name) {
                fprintf(stderr, "instantiate_prefab: ERROR - failed to duplicate component name\n");
                free(copy->id);
                free(copy);
                free_gameObject(instance);
                return NULL;
            }
        } else {
            copy->name = NULL;
        }

        copy->gameObject = instance;

        if (original->standard_voids) {
            copy->standard_voids = malloc(sizeof(component_standard_voids));
            if (!copy->standard_voids) {
                fprintf(stderr, "instantiate_prefab: ERROR - failed to allocate standard_voids\n");
                free(copy->name);
                free(copy->id);
                free(copy);
                free_gameObject(instance);
                return NULL;
            }
            memcpy(copy->standard_voids, original->standard_voids, sizeof(component_standard_voids));
        } else {
            fprintf(stderr, "instantiate_prefab: WARNING - original->standard_voids is NULL, setting copy to NULL\n");
            copy->standard_voids = NULL;
        }

        add_component(instance, copy);
    }

    return instance;
}