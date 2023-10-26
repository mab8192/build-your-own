package arkengine.scene;

import arkengine.Window;
import arkengine.rendering.Camera;
import arkengine.rendering.ImGuiLayer;

import java.util.List;
import java.util.ArrayList;

public abstract class Scene {

    public enum SceneType {
        EDITOR,
        RUNTIME
    }

    private boolean isRunning = false;
    protected List<GameObject> gameObjects = new ArrayList<>();
    protected Camera camera = new Camera();

    public Scene() { }

    // --- Functions to override
    public void init() { }

    public void start() {
        for (GameObject go : gameObjects) {
            go.start();
            Window.get().getRenderer().submit(go);
        }

        isRunning = true;
    }

    public void tick(double dt) {
        for (GameObject go : gameObjects) {
            go.tick(dt);
        }
    };

    public void imgui() { }

    // --- Functions that don't need to be overridden
    public void addGameObject(GameObject go) {
        gameObjects.add(go);

        // If game object is added during runtime, immediately start it
        // and submit it to the renderer
        if (isRunning) {
            go.start();
            Window.get().getRenderer().submit(go);
        }
    }

    /**
     * Returns the first game object found in the scene with the given name, or null if no
     * game object is found.
     * @param name
     * @return GameObject
     */
    public GameObject findGameObject(String name) {
        return gameObjects.stream().filter(go -> go.getName().equals(name)).findFirst().get();
    }

    public Camera getCamera() {
        return camera;
    }
}
