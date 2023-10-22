package arkengine.scene;

import arkengine.components.Sprite;
import arkengine.components.Transform;
import arkengine.events.KeyListener;

import arkengine.rendering.Spritesheet;
import arkengine.util.AssetPool;
import org.joml.Vector2f;
import org.joml.Vector4f;

import java.awt.event.KeyEvent;

public class LevelEditorScene extends Scene {

    @Override
    public void init() {
        loadResources();

        Spritesheet sheet = AssetPool.getSpritesheet("assets/images/testSpritesheet.png");

        GameObject obj1 = new GameObject("1", new Transform(new Vector2f(100, 100), new Vector2f(256, 256)))
                .withComponent(new Sprite(sheet, 0, 0));

        GameObject obj2 = new GameObject("2", new Transform(new Vector2f(500, 300), new Vector2f(256, 256)))
                .withComponent(new Sprite(sheet, 1, 2));

        GameObject obj3 = new GameObject("3", new Transform(new Vector2f(600, 600), new Vector2f(32, 32)))
                .withComponent(new Sprite());

        addGameObject(obj1);
        addGameObject(obj2);
        addGameObject(obj3);
    }

    private void loadResources() {
        AssetPool.getShader("assets/shaders/default.glsl");
        AssetPool.putSpritesheet(
                "assets/images/testSpritesheet.png",
                new Spritesheet(
                        AssetPool.getTexture("assets/images/testSpritesheet.png"),
                        8, 8, 0)
        );
    }

    @Override
    public void tick(double dt) {
        super.tick(dt);

        float cameraSpeed = 500f;
        if (KeyListener.isKeyPressed(KeyEvent.VK_W)) {
            camera.position.y += (float) (cameraSpeed * dt);

        }
        if (KeyListener.isKeyPressed(KeyEvent.VK_A)) {
            camera.position.x -= (float) (cameraSpeed * dt);
        }
        if (KeyListener.isKeyPressed(KeyEvent.VK_S)) {
            camera.position.y -= (float) (cameraSpeed * dt);
        }
        if (KeyListener.isKeyPressed(KeyEvent.VK_D)) {
            camera.position.x += (float) (cameraSpeed * dt);
        }
    }
}
