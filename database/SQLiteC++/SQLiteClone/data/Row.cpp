#include <iostream>
#include "Row.h"


// Object definitions
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