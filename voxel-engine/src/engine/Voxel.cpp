#include "Voxel.h"

Voxel::Voxel() : type(VoxelType::Air) {}
Voxel::~Voxel() {}
void Voxel::setType(VoxelType type) { this->type = type; }
VoxelType Voxel::getType() const { return type; }
