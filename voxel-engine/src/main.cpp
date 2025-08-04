#include "raylib.h"

#include "engine/Engine.h"

int main() {

    Config config;
    Engine engine(config);    

    if (!engine.initialize()) {
        return -1; // Initialization failed
    }

    engine.run();

    engine.shutdown();
}
