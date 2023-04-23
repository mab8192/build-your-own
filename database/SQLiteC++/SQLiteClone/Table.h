// Defines a table and other data structures used by it.
#pragma once

#include "util.h"
#include "backend.h"

struct Row {
	unsigned int id = 0;
	char username[COLUMN_USERNAME_SIZE + 1]{ 0 };
	char email[COLUMN_EMAIL_SIZE + 1]{ 0 };

	void serialize(byte* destination) const;
	static Row* deserialize(byte* source);

	void print() const;
};


struct Table {
	unsigned int num_rows = 0;
	Pager* pager;

	Table(std::string filepath);
	~Table();

	void close();
};


struct Cursor
{
	Table* table = nullptr;
	unsigned int row_num = 0;
	bool end_of_table = false;

	Cursor(Table* table, bool at_end = false);

	byte* value() const;
	void advance();
};

