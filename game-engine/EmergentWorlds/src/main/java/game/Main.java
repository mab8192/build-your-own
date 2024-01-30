package game;

import emergentworlds.Window;

public class Main {
    public static void main(String[] args) {
        Window window = Window.get();
        window.loadWorld(new MyWorld());
        window.run();
        window.destroy();
    }
}
