#include "engine/engine.h"

int main(int argc, char *argv[]) {
    if (!engine_init("atiEngine", 800, 600)) {
        return 1;
    }

    engine_run();
    engine_shutdown();

    return 0;
}