package arkengine.scene;

public abstract class Scene {

    public enum SceneType {
        LEVEL_EDITOR,
        LEVEL
    }

    public Scene() { }

    public abstract void update(double dt);
}
