#pragma once
#include <string>
#include <fstream>
#include "../utils/util.h"
#include "Page.h"

struct Pager
{
	std::fstream file_stream;
	unsigned int file_length = 0;
	Page* pages[TABLE_MAX_PAGES]{ nullptr };

	Pager(std::string filepath);

	Page* get_page(unsigned int page_num);
	void flush(unsigned int i, unsigned int size);
	void close(unsigned int num_rows);
};
