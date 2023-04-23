#include <iostream>
#include <sstream>
#include <string>

#include "util.h"
#include "backend.h"
#include "core.h"


// Helper functions
void print_prompt() {
	std::cout << "db > ";
}

int main(int argc, char* argv[]) {
	Parser parser;
	Executor executor;
	Table* table = new Table("db.bin");

	while (true) {
		print_prompt();

		// read input
		std::string cmd;
		std::getline(std::cin, cmd);

		ParseResult parse_result = parser.parse(cmd);

		switch (parse_result.status) {
		case (PARSE_SUCCESS):
			break;
		case (PARSE_SYNTAX_ERROR):
			std::cout << "Syntax error." << std::endl;
			continue;
		case (PARSE_UNRECOGNIZED_STATEMENT):
			std::cout << "Unrecognized keyword at start of: " << cmd << std::endl;
			continue;
		case (PARSE_NEGATIVE_ID):
			std::cout << "Cannot use negative IDs!" << std::endl;
			continue;
		}

		if (parse_result.type == COMMAND_TYPE_META) {
			switch (executor.do_meta_command(table, parse_result.statement)) {
			case (META_COMMAND_SUCCESS):
				continue;
			case (META_COMMAND_UNRECOGNIZED_COMMAND):
				std::cout << "Unrecognized command: " << cmd << std::endl;
				continue;
			}
		}
		else if (parse_result.type == COMMAND_TYPE_COMMAND) {
			switch (executor.execute_statement(table, parse_result.statement)) {
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
}
