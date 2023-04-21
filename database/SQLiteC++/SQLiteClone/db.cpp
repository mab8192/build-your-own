#include <iostream>
#include <sstream>
#include <string>

#include "Table.h"
#include "util.h"
#include "Cursor.h"


// Helper functions
void print_prompt() {
	std::cout << "db > ";
}

MetaCommandResult do_meta_command(Table* table, const std::string& cmd) {
	if (cmd == ".exit") {
		table->close();
		exit(EXIT_SUCCESS);
	}
	else {
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}

// These functions are essentially the virtual machine
ExecuteResult execute_insert(Table* table, const Statement& statement) {
	if (table->num_rows >= TABLE_MAX_ROWS)
		return EXECUTE_TABLE_FULL;

	Cursor cursor(table, true);

	statement.row_to_insert->serialize(cursor.value());

	table->num_rows++;

	return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Table* table, const Statement& statement) {
	Row* row;
	Cursor cursor(table);
	while (!cursor.end_of_table) {
		row = Row::deserialize(cursor.value());
		row->print();
		cursor.advance();
	}
	return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Table* table, const Statement& statement) {
	switch (statement.type) {
	case STATEMENT_UNKNOWN:
		std::cerr << "Unknown statement type!" << std::endl;
		return EXECUTE_ERROR;
	case STATEMENT_INSERT:
		return execute_insert(table, statement);
	case STATEMENT_SELECT:
		return execute_select(table, statement);
	}
}

int main(int argc, char* argv[]) {
	Table* table = new Table("db.bin");

	while (true) {
		print_prompt();

		// read input
		std::string cmd;
		std::getline(std::cin, cmd);

		if (cmd[0] == '.') {
			switch (do_meta_command(table, cmd)) {
			case (META_COMMAND_SUCCESS):
				continue;
			case (META_COMMAND_UNRECOGNIZED_COMMAND):
				std::cout << "Unrecognized command: " << cmd << std::endl;
				continue;
			}
		}

		Statement statement;

		switch (statement.prepare(cmd)) {
		case (PREPARE_SUCCESS):
			break;
		case (PREPARE_SYNTAX_ERROR):
			std::cout << "Syntax error." << std::endl;
			continue;
		case (PREPARE_UNRECOGNIZED_STATEMENT):
			std::cout << "Unrecognized keyword at start of: " << cmd << std::endl;
			continue;
		case (PREPARE_NEGATIVE_ID):
			std::cout << "Cannot use negative IDs!" << std::endl;
			continue;
		}

		switch (execute_statement(table, statement)) {
		case EXECUTE_SUCCESS:
			std::cout << "Done." << std::endl;
			break;
		case EXECUTE_ERROR:
			std::cout << "Error." << std::endl;
			break;
		case EXECUTE_TABLE_FULL:
			std::cout << "Table full!" << std::endl;
		}
		
	}
}
