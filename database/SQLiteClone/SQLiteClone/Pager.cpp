#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "Pager.h"

Pager::Pager(std::string filepath) {
	file_stream.open(filepath, std::ios::binary | std::ios::ate | std::ios::in | std::ios::out);

	if (!file_stream.is_open()) {
		printf("Unable to open file\n");
		exit(EXIT_FAILURE);
	}

	file_length = file_stream.tellg();
	printf("File is %d bytes large\n", file_length);

	file_stream.seekg(0);
}

byte* Pager::get_page(unsigned int page_num) {
	if (page_num >= TABLE_MAX_PAGES) {
		printf("Tried to fetch page number out of bounds!\n");
		exit(EXIT_FAILURE);
	}

	if (pages[page_num] == nullptr) {
		// cache miss
		byte* page = (byte*)malloc(PAGE_SIZE);
		unsigned int num_pages = file_length / PAGE_SIZE;

		if (file_length % PAGE_SIZE) {
			num_pages += 1;
		}

		if (page_num < num_pages) {
			// seek to page position in file
			file_stream.seekg(page_num * PAGE_SIZE);

			// read page from file
			file_stream.read(page, PAGE_SIZE);
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

	file_stream.write(pages[page_num], size);
	if (file_stream.fail()) {
		printf("Error writing to file\n");
		exit(EXIT_FAILURE);
	}
}
