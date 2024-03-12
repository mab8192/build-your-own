package game;

import emergentworlds.Window;
import emergentworlds.world.WorldConfiguration;

public class Main {
    public static void main(String[] args) {
        System.out.println("TEst");
        Window window = Window.get();
        window.init();
        Window.loadWorld(new MyWorld(new WorldConfiguration()
                .cellSize(4)
                .chunkWidth(4)
                .chunkHeight(4)));
        window.run();
        window.destroy();
    }
}
