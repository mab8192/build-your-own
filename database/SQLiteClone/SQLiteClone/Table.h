#pragma once
#include "util.h"
#include "Statement.h"

struct Table {
	unsigned int num_rows = 0;
	byte* pages[TABLE_MAX_PAGES]{ nullptr };

	~Table();

	byte* row_slot(unsigned int row_num);
	ExecuteResult insert(const Statement& statement);
	ExecuteResult select(const Statement& statement);
};
