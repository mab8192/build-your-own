package arkengine.scene;

import arkengine.components.SpriteRenderer;
import arkengine.components.Transform;
import arkengine.events.KeyListener;

import arkengine.util.AssetPool;
import org.joml.Vector2f;
import org.joml.Vector4f;

import java.awt.event.KeyEvent;

public class LevelEditorScene extends Scene {

    @Override
    public void init() {
        GameObject obj1 = new GameObject("1", new Transform(new Vector2f(100, 100), new Vector2f(256, 256)))
                .withComponent(new SpriteRenderer(AssetPool.getTexture("assets/images/testTex.png")));

        GameObject obj2 = new GameObject("2", new Transform(new Vector2f(500, 300), new Vector2f(256, 256)))
                .withComponent(new SpriteRenderer(AssetPool.getTexture("assets/images/testTex2.png")));

        addGameObject(obj1);
        addGameObject(obj2);

        loadResources();
    }

    private void loadResources() {
        AssetPool.getShader("assets/shaders/default.glsl");
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
