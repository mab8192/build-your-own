package arkengine.scene;

import arkengine.rendering.Sprite;
import arkengine.components.SpriteRenderer;
import arkengine.components.Transform;
import arkengine.events.Input;

import arkengine.rendering.Spritesheet;
import arkengine.util.AssetPool;
import org.joml.Vector2f;

import java.awt.event.KeyEvent;

public class LevelEditorScene extends Scene {

    GameObject obj1;
    Spritesheet sheet;

    @Override
    public void init() {
        loadResources();
        sheet = AssetPool.getSpritesheet("assets/images/testSpritesheet.png");


        obj1 = new GameObject("1", new Transform(new Vector2f(100, 100), new Vector2f(256, 256)))
                .withComponent(new SpriteRenderer(sheet.getSprite(0, 0)));

        GameObject obj2 = new GameObject("2", new Transform(new Vector2f(500, 300), new Vector2f(256, 256)))
                .withComponent(new SpriteRenderer(sheet.getSprite(0, 1)));

        GameObject obj3 = new GameObject("3", new Transform(new Vector2f(600, 600), new Vector2f(32, 32)))
                .withComponent(new SpriteRenderer());

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
                        32, 32, 0)
        );
    }

    private int spriteIndex = 0;
    private float spriteFlipTime = 0.2f;
    private float spriteFlipTimeRemaining = 0.2f;

    @Override
    public void tick(double dt) {
        super.tick(dt);

        float cameraSpeed = 500f;
        if (Input.isKeyPressed(KeyEvent.VK_W)) {
            camera.position.y += (float) (cameraSpeed * dt);
        }
        if (Input.isKeyPressed(KeyEvent.VK_A)) {
            camera.position.x -= (float) (cameraSpeed * dt);
        }
        if (Input.isKeyPressed(KeyEvent.VK_S)) {
            camera.position.y -= (float) (cameraSpeed * dt);
        }
        if (Input.isKeyPressed(KeyEvent.VK_D)) {
            camera.position.x += (float) (cameraSpeed * dt);
        }

        obj1.transform.position.x += dt*50;

        spriteFlipTimeRemaining -= dt;
        if (spriteFlipTimeRemaining <= 0) {
            spriteFlipTimeRemaining = spriteFlipTime;
            spriteIndex = spriteIndex == 0 ? 1 : 0;

             obj1.getComponent(SpriteRenderer.class).setSprite(sheet.getSprite(0, spriteIndex));
        }
    }
}
