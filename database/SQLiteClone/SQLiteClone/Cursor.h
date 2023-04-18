#pragma once
#include "Table.h"
#include "util.h"

struct Cursor
{
	Table* table = nullptr;
	uint32_t row_num = 0;
	bool end_of_table = false;

	Cursor(Table* table, bool at_end=false);

	byte* value() const;
	void advance();
};

