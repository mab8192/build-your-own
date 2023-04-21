package com.mab8192.db;

import java.nio.ByteBuffer;
import java.util.Arrays;

public class Row {
    public int id;
    public String username;
    public String email;

    public static final int ID_OFFSET = 0;
    public static final int ID_SIZE = 4;
    public static final int USERNAME_OFFSET = 4;
    public static final int USERNAME_SIZE = 32;
    public static final int EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
    public static final int EMAIL_SIZE = 255;
    public static final int ROW_SIZE = 291;

    public Row() {
        id = -1;
        username = "";
        email = "";
    }

    public Row(int id, String username, String email) {
        this.id = id;
        this.username = username;
        this.email = email;
    }

    public byte[] serialize() {
        byte[] bytes = new byte[ROW_SIZE];
        byte[] id_bytes = ByteBuffer.allocate(ID_SIZE).putInt(id).array();
        byte[] username_bytes = username.getBytes();
        byte[] email_bytes = email.getBytes();

        System.arraycopy(id_bytes, 0, bytes, 0, id_bytes.length);
        System.arraycopy(username_bytes, 0, bytes, USERNAME_OFFSET, Math.min(username_bytes.length, USERNAME_SIZE));
        System.arraycopy(email_bytes, 0, bytes, EMAIL_OFFSET, Math.min(email_bytes.length, EMAIL_SIZE));

        return bytes;
    }

    public static Row deserialize(byte[] bytes) {
        Row row = new Row();

        byte[] idBytes = new byte[ID_SIZE];
        byte[] usernameBytes = new byte[USERNAME_SIZE];
        byte[] emailBytes = new byte[EMAIL_SIZE];

        System.arraycopy(bytes, 0, idBytes, 0, ID_SIZE);
        row.id = ByteBuffer.wrap(idBytes).getInt();

        System.arraycopy(bytes, USERNAME_OFFSET, usernameBytes, 0, USERNAME_SIZE);
        row.username = new String(usernameBytes).trim();

        System.arraycopy(bytes, EMAIL_OFFSET, emailBytes, 0, EMAIL_SIZE);
        row.email = new String(emailBytes).trim();

        return row;
    }

    public String toString() {
        return String.format("(%d, %s, %s)", id, username, email);
    }
}
