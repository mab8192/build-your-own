package game;

import emergentworlds.Window;

public class Main {
    public static void main(String[] args) {
        Window window = Window.get();
        window.init();
        Window.loadWorld(new MyWorld());
        window.run();
        window.destroy();
    }
}
