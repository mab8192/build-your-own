package com.mab8192.db;

public class Parser {
    public Statement parseCommand(String cmd) {
        Statement statement = new Statement();

        if (cmd.startsWith(".")) {
            statement.type = StatementType.META;
            return statement;
        }

        String[] parts = cmd.split(" ");
        if (parts[0].equals("insert")) {
            statement.type = StatementType.INSERT;

            int id = Integer.parseInt(parts[1]);
            if (id < 0) {
                statement.type = StatementType.ERROR;
                statement.msg = "Cannot use negative IDs!";
            }
            String username = parts[2];
            String email = parts[3];
            statement.rowToInsert = new Row(id, username, email);
        } else if (parts[0].equals("select")) {
            statement.type = StatementType.SELECT;
        } else {
            statement.type = StatementType.UNSUPPORTED;
        }

        return statement;
    }
}
