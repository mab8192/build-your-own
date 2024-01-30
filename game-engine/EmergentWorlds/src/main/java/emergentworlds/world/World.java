package emergentworlds.world;

import emergentworlds.Window;
import emergentworlds.components.SpriteRenderer;
import emergentworlds.components.Transform;
import emergentworlds.rendering.Camera;
import emergentworlds.world.cell.Cell;
import org.joml.Vector2f;
import org.joml.Vector3f;

import java.util.ArrayList;
import java.util.List;

public class World {
    private boolean _isRunning = false;
    protected List<Entity> _entities = new ArrayList<>();
    protected Camera _camera = new Camera();

    private Cell[][] _cells;

    public World() { }

    // --- Functions to override
    public void init() { }

    public void start() {
        for (Entity e : _entities) {
            e.start();
            Window.get().getRenderer().submit(e);
        }

        _isRunning = true;
    }

    public void tick(double dt) {
        for (Entity go : _entities) {
            go.tick(dt);
        }
    };

    // --- Functions that don't need to be overridden
    public void addEntity(Entity e) {
        _entities.add(e);

        // If game object is added during runtime, immediately start it
        // and submit it to the renderer
        if (_isRunning) {
            e.start();
            Window.get().getRenderer().submit(e);
        }
    }

    /**
     * Returns the first game object found in the scene with the given name, or null if no
     * game object is found.
     * @param name
     * @return GameObject
     */
    public Entity findEntity(String name) {
        return _entities.stream().filter(go -> go.getName().equals(name)).findFirst().get();
    }

    public Camera getCamera() {
        return _camera;
    }

    public static World loadDefaultWorld() {
        World world = new World();
        world.addEntity(new Entity("1", new Transform(new Vector2f(100, 100), new Vector2f(256, 256)), 10)
                .withComponent(new SpriteRenderer())
        );
        return world;
    }
}
