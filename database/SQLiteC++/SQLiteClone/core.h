// Defines the parser and "virtual machine" that interprets and executes commands.
#pragma once

#include <string>

#include "util.h"
#include "Table.h"

using std::string;

// Enum return types for error handling
enum MetaCommandResult {
	META_COMMAND_SUCCESS,
	META_COMMAND_UNRECOGNIZED_COMMAND
};

enum ParseStatus {
	PARSE_SUCCESS,
	PARSE_UNRECOGNIZED_STATEMENT,
	PARSE_SYNTAX_ERROR,
	PARSE_NEGATIVE_ID
};

enum StatementType {
	STATEMENT_UNKNOWN,
	STATEMENT_INSERT,
	STATEMENT_SELECT
};

enum CommandType {
	COMMAND_TYPE_UNKNOWN,
	COMMAND_TYPE_META,
	COMMAND_TYPE_COMMAND
};

enum ExecuteResult {
	EXECUTE_SUCCESS,
	EXECUTE_ERROR,
	EXECUTE_TABLE_FULL
};

struct Statement {
	StatementType type = STATEMENT_UNKNOWN;
	string cmd = "";
	Row* row_to_insert = nullptr;

	Statement();
	Statement(string cmd);
	~Statement();
};

struct ParseResult {
	Statement statement;
	ParseStatus status = PARSE_UNRECOGNIZED_STATEMENT;
	CommandType type = COMMAND_TYPE_UNKNOWN;

	ParseResult();
	ParseResult(const string& cmd);
};

struct Parser {
	ParseResult parse_insert(const string& input);
	ParseResult parse_select(const string& input);
	ParseResult parse(const string& input);
};

struct Executor {
	MetaCommandResult do_meta_command(Table* table, Statement* statement);

	ExecuteResult execute_insert(Table* table, Statement* statement);
	ExecuteResult execute_select(Table* table, Statement* statement);
	ExecuteResult execute_statement(Table* table, Statement* statement);
};
