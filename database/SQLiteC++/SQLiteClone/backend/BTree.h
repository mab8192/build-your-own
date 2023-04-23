#pragma once
#include "../utils/util.h"

const unsigned int LEAF_NODE_MAX_CELLS = 13;

struct Node {
	bool is_root;
	Node* parent;
};

struct Cell {
	unsigned int key;
	byte value[ROW_SIZE];
};

struct LeafNode : Node {
	unsigned int num_cells = 0;
	Cell cells[LEAF_NODE_MAX_CELLS];

	unsigned int get_key(unsigned int cell_num) const;
	byte* get_value(unsigned int cell_num);
};
