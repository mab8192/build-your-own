#include "Chunk.h"

Chunk::Chunk(int width, int height, int depth)
    : width(width), height(height), depth(depth), voxels(width * height * depth) {}
Chunk::~Chunk() {}
Voxel& Chunk::getVoxel(int x, int y, int z) { return voxels[x + y * width + z * width * height]; }
void Chunk::setVoxel(int x, int y, int z, VoxelType type) { getVoxel(x, y, z).setType(type); }
int Chunk::getWidth() const { return width; }
int Chunk::getHeight() const { return height; }
int Chunk::getDepth() const { return depth; }
