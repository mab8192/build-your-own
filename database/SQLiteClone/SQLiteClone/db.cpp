#include <iostream>
#include <sstream>
#include <string>

enum MetaCommandResult {
	META_COMMAND_SUCCESS,
	META_COMMAND_UNRECOGNIZED_COMMAND
};

enum PrepareResult {
	PREPARE_SUCCESS,
	PREPARE_UNRECOGNIZED_STATEMENT
};

enum StatementType {
	STATEMENT_INSERT,
	STATEMENT_SELECT
};

struct Statement {
	StatementType type;

	PrepareResult prepare(const std::string& cmd) {
		if (cmd.substr(0, 6) == "insert") {
			type = STATEMENT_INSERT;
			return PREPARE_SUCCESS;
		}
		else if (cmd.substr(0, 6) == "select") {
			type = STATEMENT_SELECT;
			return PREPARE_SUCCESS;
		}

		return PREPARE_UNRECOGNIZED_STATEMENT;
	}
};

class InputBuffer {
private:
	std::string buffer;

public:
	void read() {
		std::getline(std::cin, buffer);
	}

	std::string str() const{
		return buffer;
	}
};

void print_prompt() {
	std::cout << "db > ";
}

MetaCommandResult do_meta_command(const std::string& cmd) {
	if (cmd == ".exit") {
		exit(EXIT_SUCCESS);
	}
	else {
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}

void execute_statement(const Statement& statement) {
	switch (statement.type) {
	case STATEMENT_INSERT:
		std::cout << "Insert ccommand.\n";
		break;
	case STATEMENT_SELECT:
		std::cout << "Select command.\n";
		break;
	}
}

int main(int argc, char* argv[]) {
	InputBuffer input_buffer;

	while (true) {
		print_prompt();
		input_buffer.read();

		std::string cmd = input_buffer.str();

		if (cmd[0] == '.') {
			switch (do_meta_command(cmd)) {
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
		case (PREPARE_UNRECOGNIZED_STATEMENT):
			std::cout << "Unrecognized keyword at start of: " << cmd << std::endl;
			continue;
		}

		execute_statement(statement);
		std::cout << "Done." << std::endl;
	}
}
