#pragma once
#include "util.h"
#include "Statement.h"
#include "Pager.h"

struct Table {
	unsigned int num_rows = 0;
	Pager* pager;

	Table(std::string filepath);
	~Table();

	byte* row_slot(unsigned int row_num);
	ExecuteResult insert(const Statement& statement);
	ExecuteResult select(const Statement& statement);

	void close();
};
