#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdbool.h>

struct GameObject;

typedef struct {
    struct GameObject *gameObject;

    char *name;
    bool isActive;

    void (*Start)(Component *self);
    void (*Update)(Component *self);
    void (*Exit)(Component *self);
} Component;