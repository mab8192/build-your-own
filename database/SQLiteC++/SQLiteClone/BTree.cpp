#include "BTree.h"

unsigned int LeafNode::get_key(unsigned int cell_num) const {
	return cells[cell_num].key;
}

byte* LeafNode::get_value(unsigned int cell_num) {
	return cells[cell_num].value;
}