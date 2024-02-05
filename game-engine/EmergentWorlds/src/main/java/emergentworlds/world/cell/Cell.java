package emergentworlds.world.cell;

import emergentworlds.rendering.CellRenderer;
import emergentworlds.world.World;
import emergentworlds.world.cell.cells.VoidCell;
import org.joml.Vector2i;

/**
 * Represents a single cell in the World
 */
public abstract class Cell {
    protected final World _world;
    protected CellType _type = CellType.VOID;
    protected CellRenderer _cellRenderer = new CellRenderer(this);
    protected Vector2i _worldPosition = new Vector2i();

    public Cell(World world) {
        this._world = world;
    }

    public Vector2i getWorldPosition() {
        return _worldPosition;
    }

    public void setWorldPosition(int x, int y) {
        setWorldPosition(new Vector2i(x, y));
    }

    public void setWorldPosition(Vector2i _worldPosition) {
        this._worldPosition = _worldPosition;
    }

    public final World getWorld() { return _world; }
    public final CellType getType() { return _type; }
    public final CellRenderer getRenderer() { return _cellRenderer; }
}
