#include <sstream>
#include <iostream>

#include "core.h"

Statement::Statement(string cmd) : cmd(cmd) {
}

Statement::~Statement() {
	if (row_to_insert != nullptr)
		delete row_to_insert;
}

// Parser
ParseResult::~ParseResult() {
	if (statement != nullptr)
		delete statement;
}

ParseResult Parser::parse_insert(const string& input) {
	ParseResult result;
	result.statement = new Statement(input);
	result.statement->type = STATEMENT_INSERT;

	result.statement->row_to_insert = new Row();

	std::stringstream stream(input);
	stream.ignore(7); // skip first 7 characters

	int id;

	stream >> id >> result.statement->row_to_insert->username >> result.statement->row_to_insert->email;

	if (id < 0) {
		result.status = PARSE_NEGATIVE_ID;
		return result;
	}
	else {
		result.statement->row_to_insert->id = id;
	}

	if (stream.fail()) {
		result.status = PARSE_SYNTAX_ERROR;
		return result;
	}

	return result;
}

ParseResult Parser::parse_select(const string& input) {
	ParseResult result;
	result.statement = new Statement(input);
	result.statement->type = STATEMENT_SELECT;
	result.status = PARSE_SUCCESS;
	return result;
}

ParseResult Parser::parse(const string& input) {
	if (input[0] == '.') {
		ParseResult result;
		return { new Statement(input), PARSE_SUCCESS, COMMAND_TYPE_META };
	}

	if (input.substr(0, 6) == "insert") {
		ParseResult result = parse_insert(input);
		result.type = COMMAND_TYPE_COMMAND;
		return result;
	}
	else if (input.substr(0, 6) == "select") {
		ParseResult result = parse_select(input);
		result.type = COMMAND_TYPE_COMMAND;
		return result;
	}

	ParseResult result;
	result.type = COMMAND_TYPE_UNKNOWN;
}

// Executor
MetaCommandResult Executor::do_meta_command(Table* table, Statement* statement) {
	if (statement->cmd == ".exit") {
		table->close();
		exit(EXIT_SUCCESS);
	}
	else {
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}

ExecuteResult Executor::execute_insert(Table* table, Statement* statement) {
	if (table->num_rows >= TABLE_MAX_ROWS)
		return EXECUTE_TABLE_FULL;

	Cursor cursor(table, true);

	statement->row_to_insert->serialize(cursor.value());

	table->num_rows++;

	return EXECUTE_SUCCESS;
}

ExecuteResult Executor::execute_select(Table* table, Statement* statement) {
	Row* row;
	Cursor cursor(table);
	while (!cursor.end_of_table) {
		row = Row::deserialize(cursor.value());
		row->print();
		cursor.advance();
	}
	return EXECUTE_SUCCESS;
}

ExecuteResult Executor::execute_statement(Table* table, Statement* statement) {
	switch (statement->type) {
	case STATEMENT_UNKNOWN:
		std::cerr << "Unknown statement type!" << std::endl;
		return EXECUTE_ERROR;
	case STATEMENT_INSERT:
		return execute_insert(table, statement);
	case STATEMENT_SELECT:
		return execute_select(table, statement);
	}

	return EXECUTE_ERROR;
}
