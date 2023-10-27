package arkengine.scene;

import arkengine.components.SpriteRenderer;
import arkengine.components.Transform;
import arkengine.events.Input;

import arkengine.rendering.Sprite;
import arkengine.rendering.Spritesheet;
import arkengine.util.AssetPool;
import imgui.ImGui;
import org.joml.Vector2f;
import org.joml.Vector3f;
import org.joml.Vector4f;

import java.awt.event.KeyEvent;

public class EditorScene extends Scene {
    GameObject selectedGameObject;

    @Override
    public void init() {
        loadResources();
        Spritesheet sheet = AssetPool.getSpritesheet("assets/images/testSpritesheet.png");
        camera.setClearColor(new Vector3f(1, 1, 1));

        GameObject obj1 = new GameObject("1", new Transform(new Vector2f(100, 100), new Vector2f(256, 256)), 10)
                .withComponent(new SpriteRenderer());

        GameObject obj2 = new GameObject("2", new Transform(new Vector2f(500, 300), new Vector2f(256, 256)))
                .withComponent(new SpriteRenderer(sheet.getSprite(0, 1)));

        addGameObject(obj1);
        addGameObject(obj2);

        selectedGameObject = obj1;
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

    private void imGuiInspector() {
        if (selectedGameObject != null) {
            SpriteRenderer spr = selectedGameObject.getComponent(SpriteRenderer.class);
            Vector4f color = spr.getColor();
            float[] imColor = new float[]{color.x, color.y, color.z, color.w};

            ImGui.begin("Inspector");
            if (ImGui.colorEdit4("Sprite Color: ", imColor)) {
                spr.setColor(imColor);
            }
            ImGui.end();
        }
    }

    @Override
    public void imgui() {
        imGuiInspector();
    }

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
    }
}
