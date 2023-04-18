#include "Table.h"

Table::Table(std::string filepath) {
	pager = new Pager(filepath);
	num_rows = pager->file_length / ROW_SIZE;
}

Table::~Table() {
	// flush contents to disk
	unsigned int num_full_pages = num_rows / ROWS_PER_PAGE;
	for (int i = 0; i < num_full_pages; i++) {
		if (pager->pages[i] == nullptr) continue;
		pager->flush(i, PAGE_SIZE);
		free(pager->pages[i]);
		pager->pages[i] = nullptr;
	}

	unsigned int num_additional_rows = num_rows % ROWS_PER_PAGE;
	if (num_additional_rows > 0) {
		unsigned int page_num = num_full_pages;
		if (pager->pages[page_num] != nullptr) {
			pager->flush(page_num, num_additional_rows * ROW_SIZE);
			free(pager->pages[page_num]);
			pager->pages[page_num] = nullptr;
		}
	}

	pager->file_stream.close();
	if (pager->file_stream.fail()) {
		printf("Failed to close file!\n");
		exit(EXIT_FAILURE);
	}

	for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
		byte* page = pager->pages[i];
		if (page) {
			free(page);
			pager->pages[i] = nullptr;
		}
	}

	delete pager;
}

byte* Table::row_slot(unsigned int row_num) {
	unsigned int page_num = row_num / ROWS_PER_PAGE;

	byte* page = pager->get_page(page_num);

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
