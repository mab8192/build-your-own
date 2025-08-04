#pragma once
#include <cstdint>

enum class VoxelType : uint8_t {
    Air = 0,
    Dirt = 1,
    Grass = 2,
    Stone = 3,
    Water = 4,
    // Add more types as needed
};

class Voxel {
public:
    Voxel();
    ~Voxel();
    void setType(VoxelType type);
    VoxelType getType() const;
private:
    VoxelType type;
};
