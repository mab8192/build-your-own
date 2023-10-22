package arkengine.scene;

import arkengine.Window;
import arkengine.rendering.Camera;

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

    public Scene() { }

    public void init() { }

    public void start() {
        for (GameObject go : gameObjects) {
            go.start();
            Window.get().getRenderer().submit(go);
        }

        isRunning = true;
    }

    public void addGameObject(GameObject go) {
        gameObjects.add(go);

        // If game object is added during runtime, immediately start it
        // and submit it to the renderer
        if (isRunning) {
            go.start();
            Window.get().getRenderer().submit(go);
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
}
