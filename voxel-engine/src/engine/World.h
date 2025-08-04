#pragma once
#include <vector>
#include "Chunk.h"

class World {
public:
    World(int chunkCountX, int chunkCountY, int chunkCountZ);
    ~World();
    Chunk& getChunk(int x, int y, int z);
    void update(float dt);
    void render();
private:
    int chunkCountX, chunkCountY, chunkCountZ;
    std::vector<Chunk> chunks;
};
