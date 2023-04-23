#include "Cursor.h"

Cursor::Cursor(Table* table, bool at_end) 
	: table(table), end_of_table(at_end) {
	if (at_end) {
		row_num = table->num_rows;
	}
	else {
		row_num = 0;
	}
}

byte* Cursor::value() const {
	unsigned int page_num = row_num / ROWS_PER_PAGE;

	Page* page = table->pager->get_page(page_num);

	unsigned int row_offset = row_num % ROWS_PER_PAGE;
	unsigned int byte_offset = row_offset * ROW_SIZE;
	return page->data + byte_offset;
}

void Cursor::advance() {
	row_num++;
	if (row_num >= table->num_rows) {
		end_of_table = true;
	}
}
