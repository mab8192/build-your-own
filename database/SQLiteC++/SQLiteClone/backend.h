// Defines the B-Tree and pager used by the system to index pages of memory and interface with the filesystem
#pragma once

#include <fstream>
#include <string>

#include "util.h"

using std::string;

struct Page {
	byte data[PAGE_SIZE];
};

struct Pager
{
	std::fstream file_stream;
	unsigned int file_length = 0;
	Page* pages[TABLE_MAX_PAGES]{ nullptr };

	Pager(string filepath);

	Page* get_page(unsigned int page_num);
	void flush(unsigned int i, unsigned int size);
	void close(unsigned int num_rows);
};
