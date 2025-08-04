#include "World.h"

World::World(int chunkCountX, int chunkCountY, int chunkCountZ)
    : chunkCountX(chunkCountX), chunkCountY(chunkCountY), chunkCountZ(chunkCountZ),
      chunks(chunkCountX * chunkCountY * chunkCountZ, Chunk(16, 16, 16)) {}
World::~World() {}
Chunk& World::getChunk(int x, int y, int z) { return chunks[x + y * chunkCountX + z * chunkCountX * chunkCountY]; }
void World::update(float dt) {}
void World::render() {}
