#pragma once
#include <vector>
#include "Voxel.h"

class Chunk {
public:
    Chunk(int width, int height, int depth);
    ~Chunk();
    Voxel& getVoxel(int x, int y, int z);
    void setVoxel(int x, int y, int z, VoxelType type);
    int getWidth() const;
    int getHeight() const;
    int getDepth() const;
private:
    int width, height, depth;
    std::vector<Voxel> voxels;
};
