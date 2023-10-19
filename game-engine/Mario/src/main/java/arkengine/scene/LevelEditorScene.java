package arkengine.scene;

import arkengine.events.KeyListener;
import arkengine.Window;

import java.awt.event.KeyEvent;

public class LevelEditorScene extends Scene {
    private boolean changingScene = false;
    private final double animationLength = 2.0f;
    private double sceneChangeAnimationTimeRemaining = animationLength;

    public LevelEditorScene() {
        System.out.println("Created new LevelEditorScene");
        Window.get().r = 1;
        Window.get().g = 0;
        Window.get().b = 0;
    }

    @Override
    public void update(double dt) {
        System.out.println("Running at: " + 1.0f/dt + " fps with delta time " + dt);

        if (!changingScene && KeyListener.isKeyPressed(KeyEvent.VK_SPACE)) {
            changingScene = true;
        }

        if (changingScene && sceneChangeAnimationTimeRemaining > 0) {
            sceneChangeAnimationTimeRemaining -= dt;
            Window.get().r = (float) Math.max(Window.get().r - dt/animationLength, 0.0f);
            Window.get().g = (float) Math.max(Window.get().g - dt/animationLength, 0.0f);
            Window.get().b = (float) Math.max(Window.get().b - dt/animationLength, 0.0f);
        } else if (changingScene && sceneChangeAnimationTimeRemaining < 0) {
            // Changing scene is complete
            Window.changeScene(SceneType.LEVEL);
        }
    }
}
