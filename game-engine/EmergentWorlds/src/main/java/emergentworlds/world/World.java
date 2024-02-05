package emergentworlds.world;

import emergentworlds.Window;
import emergentworlds.components.SpriteRenderer;
import emergentworlds.components.Transform;
import emergentworlds.rendering.Camera;
import emergentworlds.rendering.Renderer;
import emergentworlds.world.cell.Cell;
import emergentworlds.world.cell.CellType;
import emergentworlds.world.cell.cells.VoidCell;
import org.joml.Vector2f;

import java.util.ArrayList;
import java.util.List;

public class World {
    private boolean _isRunning = false;
    protected List<Entity> _entities = new ArrayList<>();
    protected Camera _camera = new Camera();

    private Renderer renderer;

    protected int _worldWidth, _worldHeight; // Defaults
    protected float _cellSize;
    protected Cell[][] _cells;

    private boolean cellsInitialized = false;

    public World() { }

    // --- Functions to override
    /**
     * Perform initialization for the world. Load resources, prep entities/cells, etc.
     */
    public void init() { }

    /**
     * Start the World.
     */
    public final void start() {
        if (!cellsInitialized) {
            throw new RuntimeException("Must initialize cells first by calling setWorldSize!");
        }

        renderer = Window.getRenderer();

        for (Entity e : _entities) {
            e.start();
            renderer.submit(e);
        }

        for (Cell[] row : _cells) {
            for (Cell c : row) {
                if (c.getType() != CellType.VOID)
                    renderer.submit(c);
            }
        }

        _isRunning = true;
    }

    public void tick(double dt) {
        for (Entity go : _entities) {
            go.tick(dt);
        }
    };

    // --- Functions that don't need to be overridden
    protected final void initCells() {
        for (int i = 0; i < _worldWidth; i++) {
            for (int j = 0; j < _worldHeight; j++) {
                _cells[j][i] = new VoidCell(this);
            }
        }
    }

    public final void setCellSize(int size) {
        _cellSize = size;
    }

    public final float getCellSize() {
        return _cellSize;
    }

    public final void setWorldSize(int width, int height) {
        _worldWidth = width;
        _worldHeight = height;
        _cells = new Cell[_worldHeight][_worldWidth];

        if (!cellsInitialized) {
            initCells();
            cellsInitialized = true;
        } else {
            // TODO: Copy cells over to new world
        }
    }

    public final void setCell(int x, int y, Cell cell) {
        // Remove the old cell from the renderer
        if (_isRunning)
            renderer.remove(_cells[y][x].getRenderer());

        // Update the world with the new cell
        cell.setWorldPosition(x, y);
        _cells[y][x] = cell;

        // Update the renderer
        if (_isRunning)
            renderer.submit(cell.getRenderer());
    }

    public final Cell getCell(int x, int y) {
        return _cells[y][x];
    }

    public final void addEntity(Entity e) {
        _entities.add(e);

        // If game object is added during runtime, immediately start it
        // and submit it to the renderer
        if (_isRunning) {
            e.start();
            renderer.submit(e);
        }
    }

    /**
     * Returns the first game object found in the scene with the given name, or null if no
     * game object is found.
     * @param name
     * @return GameObject
     */
    public Entity findEntity(String name) {
        return _entities.stream()
                .filter(go -> go.getName().equals(name))
                .findFirst()
                .orElseGet(() -> null);
    }

    public Camera getCamera() {
        return _camera;
    }

    public static World loadDefaultWorld() {
        World world = new World();
        world.setCellSize(1);
        world.setWorldSize(100, 100);
        world.addEntity(new Entity("1", new Transform(new Vector2f(100, 100), new Vector2f(256, 256)), 10)
                .withComponent(new SpriteRenderer())
        );
        return world;
    }
}
