#pragma once
#include "util.h"
#include "Statement.h"
#include "Pager.h"

struct Table {
	unsigned int num_rows = 0;
	Pager* pager;

	Table(std::string filepath);
	~Table();

	void close();
};
