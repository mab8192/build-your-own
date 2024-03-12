package emergentworlds.world.cell;

import emergentworlds.rendering.CellRenderer;
import emergentworlds.world.ITickable;
import org.joml.Vector2i;

/**
 * Represents a single cell in the World
 */
public abstract class Cell implements ITickable {
    protected CellType _type = CellType.VOID;
    protected CellRenderer _cellRenderer = new CellRenderer(this);
    protected Vector2i _worldPosition = new Vector2i();
    protected boolean _tickable = false; // all cells are not tickable by default

    public Cell() { }

    public void tick(double dt) { }

    public Vector2i getWorldPosition() {
        return _worldPosition;
    }
    public void setWorldPosition(Vector2i _worldPosition) {
        this._worldPosition = _worldPosition;
    }

    public boolean isTickable() { return _tickable; }

    public final CellType getType() { return _type; }
    public final CellRenderer getRenderer() { return _cellRenderer; }
}
