#pragma once
#include <string>
#include <sstream>
#include "Row.h"
#include "../utils/util.h"

struct Statement {
	StatementType type = STATEMENT_UNKNOWN;
	Row* row_to_insert = nullptr;

	~Statement();

	PrepareResult prepare_insert(const std::string& cmd);
	PrepareResult prepare(const std::string& cmd);
};
