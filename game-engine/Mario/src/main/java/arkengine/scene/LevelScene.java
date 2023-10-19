package arkengine.scene;

import arkengine.Window;
import arkengine.events.KeyListener;

import java.awt.event.KeyEvent;

public class LevelScene extends Scene {
    public LevelScene() {
        System.out.println("Created new LevelScene");
        Window.get().r = 1;
        Window.get().g = 1;
        Window.get().b = 1;
    }

    @Override
    public void update(double dt) {
        if (KeyListener.isKeyPressed(KeyEvent.VK_SPACE))
            Window.changeScene(SceneType.LEVEL_EDITOR);
    }
}
