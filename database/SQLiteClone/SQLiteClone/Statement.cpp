#include "Statement.h"


Statement::~Statement() {
	if (row_to_insert != nullptr)
		delete row_to_insert;
}

PrepareResult Statement::prepare_insert(const std::string& cmd) {
	type = STATEMENT_INSERT;

	row_to_insert = new Row();

	std::stringstream stream(cmd);
	stream.ignore(7); // skip first 7 characters

	int id;

	stream >> id >> row_to_insert->username >> row_to_insert->email;

	if (id < 0) {
		return PREPARE_NEGATIVE_ID;
	}
	else {
		row_to_insert->id = id;
	}

	if (stream.fail()) {
		return PREPARE_SYNTAX_ERROR;
	}

	return PREPARE_SUCCESS;
}

PrepareResult Statement::prepare(const std::string& cmd) {
	if (cmd.substr(0, 6) == "insert") {
		return prepare_insert(cmd);
	}
	else if (cmd.substr(0, 6) == "select") {
		type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	}

	return PREPARE_UNRECOGNIZED_STATEMENT;
}
