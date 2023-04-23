#pragma once

// Custom Types
typedef char byte;

// Constants
constexpr unsigned int COLUMN_USERNAME_SIZE = 32;
constexpr unsigned int COLUMN_EMAIL_SIZE = 255;
constexpr unsigned int PAGE_SIZE = 4096;
constexpr unsigned int TABLE_MAX_PAGES = 100;

const unsigned int ID_SIZE = 4;
const unsigned int USERNAME_SIZE = COLUMN_USERNAME_SIZE;
const unsigned int EMAIL_SIZE = COLUMN_EMAIL_SIZE;
const unsigned int ID_OFFSET = 0;
const unsigned int USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const unsigned int EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const unsigned int ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const unsigned int ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const unsigned int TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;


