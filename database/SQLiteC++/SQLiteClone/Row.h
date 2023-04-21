#pragma once

#include "util.h"

// Object definitions
struct Row {
	unsigned int id = 0;
	char username[COLUMN_USERNAME_SIZE + 1]{ 0 };
	char email[COLUMN_EMAIL_SIZE + 1]{ 0 };

	void serialize(byte* destination) const;
	static Row* deserialize(byte* source);

	void print() const;
};
