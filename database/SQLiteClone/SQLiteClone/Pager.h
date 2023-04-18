#pragma once
#include <string>
#include <fstream>
#include "util.h"

struct Pager
{
	std::fstream file_stream;
	unsigned int file_length = 0;
	byte* pages[TABLE_MAX_PAGES]{ nullptr };

	Pager(std::string filepath);
	// no destructor, pages are freed manually by the table. (ew)

	byte* get_page(unsigned int page_num);
	void flush(unsigned int i, unsigned int size);
};
