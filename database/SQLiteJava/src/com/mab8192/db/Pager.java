package com.mab8192.db;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;

public class Pager {
    private RandomAccessFile file;
    public Table table;

    public Pager(String filepath, Table table) {
        this.table = table;

        try {
            File dbFile = new File(filepath);
            if (!dbFile.exists()) {
                dbFile.createNewFile();
            }
            this.file = new RandomAccessFile(filepath, "rw");
            byte[] fileContents = new byte[(int) file.length()];
            this.file.readFully(fileContents);

            for (int i = 0; i< fileContents.length; i += Row.ROW_SIZE) {
                byte[] rowBytes = new byte[Row.ROW_SIZE];
                int length = Math.min(Row.ROW_SIZE, fileContents.length - i);
                System.arraycopy(fileContents, i, rowBytes, 0, Row.ROW_SIZE);
                Row row = Row.deserialize(rowBytes);
                table.insert(row);
            }

            System.out.printf("Loaded db file: %d bytes\n", this.file.length());
        } catch (IOException e) {
            System.err.println("Failed to open db file!");
            System.exit(1);
        }
    }

    public void close() {
        // write tables to file
        ArrayList<Row> rows = table.select();
        try {
            this.file.seek(0);

            for (Row row : rows) {
                this.file.write(row.serialize());
            }

            this.file.close();

        } catch (IOException e) {
            System.err.println("Failed to write db file!");
            System.exit(1);
        }
    }
}
