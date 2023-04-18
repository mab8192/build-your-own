#include "Table.h"

Table::Table(std::string filepath) {
	pager = new Pager(filepath);
	num_rows = pager->file_length / ROW_SIZE;
}

Table::~Table() {
	delete pager;
}

byte* Table::row_slot(unsigned int row_num) {
	unsigned int page_num = row_num / ROWS_PER_PAGE;

	Page* page = pager->get_page(page_num);

	unsigned int row_offset = row_num % ROWS_PER_PAGE;
	unsigned int byte_offset = row_offset * ROW_SIZE;
	return page->data + byte_offset;
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


void Table::close() {
	// flush contents to disk
	pager->close(num_rows);
}