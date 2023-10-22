package arkengine.scene;

import arkengine.rendering.Camera;
import arkengine.GameObject;
import arkengine.rendering.Renderer;

import java.util.List;
import java.util.ArrayList;

public abstract class Scene {

    public enum SceneType {
        LEVEL_EDITOR,
        LEVEL
    }

    private boolean isRunning = false;
    protected List<GameObject> gameObjects = new ArrayList<>();
    protected Camera camera = new Camera();
    protected Renderer renderer = new Renderer();

    public Scene() { }

    public void init() { }

    public void start() {
        for (GameObject go : gameObjects) {
            go.start();
            renderer.submit(go);
        }

        isRunning = true;
    }

    public void addGameObject(GameObject go) {
        gameObjects.add(go);

        if (isRunning) {
            go.start();
            renderer.submit(go);
        }
    }

    public Camera getCamera() {
        return camera;
    }

    public void tick(double dt) {
        for (GameObject go : gameObjects) {
            go.tick(dt);
        }
    };

    public final void render() {
        renderer.render();
    }
}
