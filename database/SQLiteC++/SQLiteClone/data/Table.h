#pragma once
#include "../utils/util.h"
#include "Statement.h"
#include "../backend/Pager.h"

struct Table {
	unsigned int num_rows = 0;
	Pager* pager;

	Table(std::string filepath);
	~Table();

	void close();
};
