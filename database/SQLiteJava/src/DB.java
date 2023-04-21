import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

import com.mab8192.db.*;

public class DB {
    public static void printPrompt() {
        System.out.println("db > ");
    }

    public static void executeMetaCommand(String cmd, Pager pager) {
        if (cmd.equals(".exit")) {
            pager.close();
            System.exit(0);
        } else {
            System.err.println("Unsupported meta command: " + cmd);
        }
    }

    public static void main(String[] args) {
        Table table = new Table();
        Pager pager = new Pager("db.bin", table);
        Parser parser = new Parser();
        Scanner scanner = new Scanner(System.in);

        while (true) {
            printPrompt();
            String cmd = scanner.nextLine();
            if (cmd.equals("")) continue;

            Statement statement = parser.parseCommand(cmd);
            switch (statement.type) {
                case ERROR -> {
                    System.err.println(statement.msg);
                }
                case META -> {
                    executeMetaCommand(cmd, pager);
                }
                case UNSUPPORTED -> {
                    System.err.println("Unsupported statement: " + cmd);
                }
                case INSERT -> {
                    table.insert(statement.rowToInsert);
                }
                case SELECT -> {
                    ArrayList<Row> rows = table.select(statement);
                    for (Row r : rows) {
                        System.out.println(r);
                    }
                }
            }
        }
    }
}
