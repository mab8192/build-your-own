#include "Table.h"

Table::~Table() {
	for (int i = 0; i < TABLE_MAX_PAGES; i++) {
		if (pages[i] == nullptr) continue;
		delete pages[i];
	}
}

byte* Table::row_slot(unsigned int row_num) {
	unsigned int page_num = row_num / ROWS_PER_PAGE;

	byte* page = pages[page_num];
	if (page == nullptr) {
		// allocate memory when we try to access an uninitialized page.
		page = pages[page_num] = (byte*)malloc(PAGE_SIZE);
	}

	unsigned int row_offset = row_num % ROWS_PER_PAGE;
	unsigned int byte_offset = row_offset * ROW_SIZE;
	return page + byte_offset;
}

ExecuteResult Table::insert(const Statement& statement) {
	if (num_rows >= TABLE_MAX_ROWS)
		return EXECUTE_TABLE_FULL;

	statement.row_to_insert->serialize(row_slot(num_rows));

	num_rows++;

	return EXECUTE_SUCCESS;
}

ExecuteResult Table::select(const Statement& statement) {
	Row* row;
	for (unsigned int i = 0; i < num_rows; i++) {
		row = Row::deserialize(row_slot(i));
		row->print();
	}
	return EXECUTE_SUCCESS;
}
