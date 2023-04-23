#include <iostream>

#include "Table.h"

// Row
void Row::serialize(byte* destination) const {
	memcpy(destination + ID_OFFSET, &id, ID_SIZE);
	memcpy(destination + USERNAME_OFFSET, username, USERNAME_SIZE);
	memcpy(destination + EMAIL_OFFSET, email, EMAIL_SIZE);
}

Row* Row::deserialize(byte* source) {
	Row* row = new Row();
	memcpy(&row->id, source + ID_OFFSET, ID_SIZE);
	memcpy(row->username, source + USERNAME_OFFSET, USERNAME_SIZE);
	memcpy(row->email, source + EMAIL_OFFSET, EMAIL_SIZE);

	return row;
}

void Row::print() const {
	std::cout << "(" << id << ", " << username << ", " << email << ")\n";
}

// Table
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

// Cursor
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
