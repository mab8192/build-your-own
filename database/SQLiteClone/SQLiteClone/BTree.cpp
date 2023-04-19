#include "BTree.h"

unsigned int LeafNode::get_key(unsigned int cell_num) const {
	return cells[cell_num].key;
}

byte[291] LeafNode::get_value(unsigned int cell_num) const {
	return cells[cell_num].value;
}