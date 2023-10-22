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
        int xOffset = 10;
        int yOffset = 10;

        float totalWidth = (float)(600 - xOffset * 2);
        float totalHeight = (float)(600 - yOffset * 2);

        float sizeX = totalWidth / 100.0f;
        float sizeY = totalHeight / 100.0f;

        for (int x = 0; x < 100; x++) {
            for (int y = 0; y < 100; y++) {
                float xPos = xOffset + (x * sizeX);
                float yPos = yOffset + (y * sizeY);

                GameObject go = new GameObject("Obj " + x + "" + y, new Transform(new Vector2f(xPos, yPos), new Vector2f(sizeX, sizeY)));
                go.addComponent(new SpriteRenderer(new Vector4f(xPos / totalWidth, yPos / totalHeight, 1, 1)));
                addGameObject(go);
            }
        }

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
