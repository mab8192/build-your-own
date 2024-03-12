package emergentworlds.world;

import emergentworlds.Window;
import emergentworlds.rendering.Camera;
import emergentworlds.rendering.Renderer;
import emergentworlds.world.cell.Cell;
import org.joml.Vector2i;

import java.util.ArrayList;
import java.util.Iterator;

/**
 * The game world.
 * This is the primary API for all objects to learn about other objects. The World is also responsible for
 * ticking time forward.
 */
public class World {
    private static World activeWorld = null;

    private boolean isRunning = false;
    protected Camera camera = new Camera();

    private Renderer renderer;

    private final WorldConfiguration config;
    private final CellManager cellManager;
//    private final EntityManager entityManager;

    private ArrayList<ITickable> tickables = new ArrayList<>();

    public World(WorldConfiguration config) {
        this.config = config;
        cellManager = new CellManager(config.chunkWidth(), config.chunkHeight());
//        entityManager = null;

        if (activeWorld == null) activeWorld = this;
    }

    // --- Functions to override
    /**
     * Perform initialization for the world. Load resources, prep entities/cells, etc.
     */
    public void init() { }

    public void tick(double dt) {
        for (ITickable tickable : tickables) tickable.tick(dt);
    };

    // --- Functions that don't need to be overridden
    public static final World getActiveWorld() {
        return World.activeWorld;
    }

    /**
     * Start the World.
     */
    public final void start() {
        if (this.config == null) throw new IllegalStateException("Cannot start the world without a configuration.");

        renderer = Window.getRenderer();

//        for (Entity e : _entities) {
//            e.start();
//            renderer.submit(e);
//        }

        Iterator<Cell> cells = cellManager.cellIterator();
        while (cells.hasNext()) {
            Cell cell = cells.next();
            renderer.submit(cell);
        }

        isRunning = true;
        World.activeWorld = this;
    }

    public final float getCellSize() {
        return this.config.cellSize();
    }

    public final void setCell(Vector2i position, Cell cell) {
        // Remove the old cell from the renderer
        if (isRunning)
            renderer.remove(cellManager.getCell(position).getRenderer());

        // Update the world with the new cell
        cell.setWorldPosition(position);
        cellManager.setCell(position, cell);

        // Update the renderer
        if (isRunning)
            renderer.submit(cell.getRenderer());

        if (cell.isTickable()) tickables.add(cell);
    }

    public final Cell getCell(Vector2i position) {
        return cellManager.getCell(position);
    }

//    public final void addEntity(Entity e) {
//        _entities.add(e);
//
//        // If game object is added during runtime, immediately start it
//        // and submit it to the renderer
//        if (isRunning) {
//            e.start();
//            renderer.submit(e);
//        }
//    }

//    public Entity findEntity(String name) {
//        return _entities.stream()
//                .filter(go -> go.getName().equals(name))
//                .findFirst()
//                .orElseGet(() -> null);
//    }

    public Camera getCamera() {
        return camera;
    }

    public static World loadDefaultWorld() {
        return new World(WorldConfiguration.DEFAULT);
    }
}
