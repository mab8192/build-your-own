package com.mab8192.db;

import java.util.ArrayList;

public class Table {
    // TODO: user better data structure
    public ArrayList<Row> rows;

    public Table() {
        rows = new ArrayList<>();
    }

    public void insert(Row row) {
        rows.add(row);
    }

    public void delete(Statement statement) {
        // TODO
        System.out.println("Handle delete!");
    }

    public ArrayList<Row> select() {
        return rows;
    }

    public ArrayList<Row> select(Statement statement) {
        // TODO: apply statement to filter rows
        return rows;
    }
}
