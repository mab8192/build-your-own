#include <iostream>
#include <sstream>
#include <string>

#include "Table.h"
#include "util.h"


// Helper functions
void print_prompt() {
	std::cout << "db > ";
}

MetaCommandResult do_meta_command(const std::string& cmd, Table& table) {
	if (cmd == ".exit") {
		table.close();
		exit(EXIT_SUCCESS);
	}
	else {
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}

ExecuteResult execute_statement(const Statement& statement, Table& table) {
	switch (statement.type) {
	case STATEMENT_UNKNOWN:
		std::cerr << "Unknown statement type!" << std::endl;
		return EXECUTE_ERROR;
	case STATEMENT_INSERT:
		return table.insert(statement);
	case STATEMENT_SELECT:
		return table.select(statement);
	}
}

int main(int argc, char* argv[]) {
	Table table("db.bin");

	while (true) {
		print_prompt();

		// read input
		std::string cmd;
		std::getline(std::cin, cmd);

		if (cmd[0] == '.') {
			switch (do_meta_command(cmd, table)) {
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

		switch (execute_statement(statement, table)) {
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
