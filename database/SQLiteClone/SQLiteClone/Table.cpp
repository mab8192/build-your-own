#include "Table.h"

Table::Table(std::string filepath) {
	pager = new Pager(filepath);
	num_rows = pager->file_length / ROW_SIZE;
}

Table::~Table() {
	delete pager;
}

void Table::close() {
	// flush contents to disk
	pager->close(num_rows);
}