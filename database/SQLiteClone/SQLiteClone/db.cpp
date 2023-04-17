#include <iostream>
#include <sstream>
#include <string>

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

typedef char byte;

// Enum return types for error handling
enum MetaCommandResult {
	META_COMMAND_SUCCESS,
	META_COMMAND_UNRECOGNIZED_COMMAND
};

enum PrepareResult {
	PREPARE_SUCCESS,
	PREPARE_UNRECOGNIZED_STATEMENT,
	PREPARE_SYNTAX_ERROR
};

enum StatementType {
	STATEMENT_UNKNOWN,
	STATEMENT_INSERT,
	STATEMENT_SELECT
};

enum ExecuteResult {
	EXECUTE_SUCCESS,
	EXECUTE_ERROR,
	EXECUTE_TABLE_FULL
};

// forward declarations for objects
// TODO: move these to their own files
struct Row;
struct Statement;
struct Table;

// Object definitions
struct Row {
	unsigned int id = 0;
	char username[COLUMN_USERNAME_SIZE]{ 0 };
	char email[COLUMN_EMAIL_SIZE]{ 0 };

	void serialize(byte* destination) const {
		memcpy(destination + ID_OFFSET, &id, ID_SIZE);
		memcpy(destination + USERNAME_OFFSET, username, USERNAME_SIZE);
		memcpy(destination + EMAIL_OFFSET, email, EMAIL_SIZE);
	}

	static Row* deserialize(byte* source) {
		Row* row = new Row();
		memcpy(&row->id, source + ID_OFFSET, ID_SIZE);
		memcpy(row->username, source + USERNAME_OFFSET, USERNAME_SIZE);
		memcpy(row->email, source + EMAIL_OFFSET, EMAIL_SIZE);

		return row;
	}

	void print() const {
		std::cout << "(" << id << ", " << username << ", " << email << ")\n";
	}
};

struct Statement {
	StatementType type = STATEMENT_UNKNOWN;
	Row* row_to_insert = nullptr;

	~Statement() {
		if (row_to_insert != nullptr)
			delete row_to_insert;
	}

	PrepareResult prepare(const std::string& cmd) {
		if (cmd.substr(0, 6) == "insert") {
			type = STATEMENT_INSERT;

			row_to_insert = new Row();

			std::stringstream stream(cmd);

			stream.ignore(7);
			stream >> row_to_insert->id >> row_to_insert->username >> row_to_insert->email;

			if (stream.fail())
				return PREPARE_SYNTAX_ERROR;

			return PREPARE_SUCCESS;
		}
		else if (cmd.substr(0, 6) == "select") {
			type = STATEMENT_SELECT;
			return PREPARE_SUCCESS;
		}

		return PREPARE_UNRECOGNIZED_STATEMENT;
	}
};

struct Table {
	unsigned int num_rows = 0;
	byte* pages[TABLE_MAX_PAGES]{ nullptr };

	~Table() {
		for (int i = 0; i < TABLE_MAX_PAGES; i++) {
			if (pages[i] == nullptr) continue;
			delete pages[i];
		}
	}

	byte* row_slot(unsigned int row_num) {
		unsigned int page_num = row_num / ROWS_PER_PAGE;

		byte* page = pages[page_num];
		if (page == nullptr) {
			// allocate memory when we try to access an uninitialized page.
			page = pages[page_num] = (byte*) malloc(PAGE_SIZE);
		}

		unsigned int row_offset = row_num % ROWS_PER_PAGE;
		unsigned int byte_offset = row_offset * ROW_SIZE;
		return page + byte_offset;
	}

	ExecuteResult insert(const Statement& statement) {
		if (num_rows >= TABLE_MAX_ROWS)
			return EXECUTE_TABLE_FULL;

		statement.row_to_insert->serialize(row_slot(num_rows));
		
		num_rows++;

		return EXECUTE_SUCCESS;
	}

	ExecuteResult select(const Statement& statement) {
		Row* row;
		for (unsigned int i = 0; i < num_rows; i++) {
			row = Row::deserialize(row_slot(i));
			row->print();
		}
		return EXECUTE_SUCCESS;
	}
};

// Helper functions
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
	Table table;

	while (true) {
		print_prompt();

		// read input
		std::string cmd;
		std::getline(std::cin, cmd);

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
		case (PREPARE_SYNTAX_ERROR):
			std::cout << "Syntax error." << std::endl;
			continue;
		case (PREPARE_UNRECOGNIZED_STATEMENT):
			std::cout << "Unrecognized keyword at start of: " << cmd << std::endl;
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
