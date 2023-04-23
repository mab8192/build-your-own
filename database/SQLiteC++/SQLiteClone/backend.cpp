#include "backend.h"


Pager::Pager(std::string filepath) {
	// This creates the file if it doesn't already exist by opening it in append mode
	file_stream.open(filepath, std::ios::app);
	if (!file_stream.is_open()) {
		printf("Unable to open file\n");
		exit(EXIT_FAILURE);
	}
	file_stream.close();

	// reopen the file in the appropriate mode
	file_stream.open(filepath, std::ios::binary | std::ios::ate | std::ios::in | std::ios::out);

	if (!file_stream.is_open()) {
		printf("Unable to open file\n");
		exit(EXIT_FAILURE);
	}

	file_length = file_stream.tellg();
	printf("File is %d bytes large\n", file_length);

	file_stream.seekg(0);
}

Page* Pager::get_page(unsigned int page_num) {
	if (page_num >= TABLE_MAX_PAGES) {
		printf("Tried to fetch page number out of bounds!\n");
		exit(EXIT_FAILURE);
	}

	if (pages[page_num] == nullptr) {
		// cache miss, allocate another page
		Page* page = new Page();
		unsigned int num_pages = file_length / PAGE_SIZE;

		if (file_length % PAGE_SIZE) {
			num_pages += 1;
		}

		if (page_num < num_pages) {
			// seek to page position in file
			file_stream.seekg(page_num * PAGE_SIZE);

			// read page from file
			file_stream.read(page->data, PAGE_SIZE);
			int bytes_read = file_stream.gcount();
			if (bytes_read > 0) {
				file_stream.clear();
			}

			if (file_stream.fail()) {
				printf("Error reading file!\n");
				exit(EXIT_FAILURE);
			}
		}

		pages[page_num] = page;
	}

	return pages[page_num];
}

void Pager::flush(unsigned int page_num, unsigned int size) {
	if (pages[page_num] == nullptr) {
		printf("Tried to flush a null page!\n");
		exit(EXIT_FAILURE);
	}

	file_stream.seekp(page_num * PAGE_SIZE);
	if (file_stream.fail()) {
		printf("Failed to seek\n");
		exit(EXIT_FAILURE);
	}

	file_stream.write(pages[page_num]->data, size);
	if (file_stream.fail()) {
		printf("Error writing to file\n");
		exit(EXIT_FAILURE);
	}
}

void Pager::close(unsigned int num_rows) {
	unsigned int num_full_pages = num_rows / ROWS_PER_PAGE;
	for (unsigned int i = 0; i < num_full_pages; i++) {
		if (pages[i] == nullptr) continue;
		flush(i, PAGE_SIZE);
		delete pages[i];
		pages[i] = nullptr;
	}

	unsigned int num_additional_rows = num_rows % ROWS_PER_PAGE;
	if (num_additional_rows > 0) {
		unsigned int page_num = num_full_pages;
		if (pages[page_num] != nullptr) {
			flush(page_num, num_additional_rows * ROW_SIZE);
			delete pages[page_num];
			pages[page_num] = nullptr;
		}
	}

	file_stream.close();
	if (file_stream.fail()) {
		printf("Failed to close file!\n");
		exit(EXIT_FAILURE);
	}

	// delete remaining pages, if any
	for (unsigned int i = 0; i < TABLE_MAX_PAGES; i++) {
		Page* page = pages[i];
		if (page != nullptr) {
			delete page;
			pages[i] = nullptr;
		}
	}
}
