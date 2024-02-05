package emergentworlds.rendering;

import emergentworlds.util.Color;
import emergentworlds.world.cell.Cell;
import org.joml.Vector2f;
import org.joml.Vector4f;

public class CellRenderer implements IRenderable {
    private Sprite _sprite;
    private Vector4f _color;

    private final Cell _cell;

    public static final int CELL_Z_INDEX = 10;

    private boolean dirty = true; // whether the data should be reloaded for rendering

    public CellRenderer(Cell parent) {
        this(parent, new Sprite(), Color.WHITE);
    }

    public CellRenderer(Cell parent, Sprite sprite) {
        this(parent, sprite, Color.WHITE);
    }

    public CellRenderer(Cell parent, Vector4f color) {
        this(parent, new Sprite(), color);
    }

    public CellRenderer(Cell parent, Sprite sprite, Vector4f color) {
        this._cell = parent;
        this._sprite = sprite;
        this._color = color;
    }

    public Cell getCell() {
        return _cell;
    }

    @Override
    public Vector2f getWorldPosition() {
        float x = _cell.getWorldPosition().x * _cell.getWorld().getCellSize();
        float y = _cell.getWorldPosition().y * _cell.getWorld().getCellSize();
        return new Vector2f(x, y);
    }

    @Override
    public Vector2f getOffset() {
        return new Vector2f();
    }

    @Override
    public Vector4f getColor() {
        return _color;
    }

    @Override
    public Vector2f getScale() {
        return new Vector2f(_cell.getWorld().getCellSize(), _cell.getWorld().getCellSize());
    }

    public void setColor(Vector4f _color) {
        this._color = _color;
        setDirty();
    }

    public void setColor(float[] color) {
        this._color.set(color);
        setDirty();
    }

    @Override
    public Texture getTexture() {
        if (_sprite != null)
            return _sprite.getTexture();
        return null;
    }

    public Vector2f[] getTexCoords() {
        if (_sprite != null)
            return _sprite.getTexCoords();
        return Texture.DEFAULT_TEX_COORDS;
    }

    public Sprite getSprite() {
        return _sprite;
    }

    public void setSprite(Sprite _sprite) {
        this._sprite = _sprite;
        setDirty();
    }

    public void setClean() {
        dirty = false;
    }

    public void setDirty() {
        dirty = true;
    }

    public boolean isDirty() {
        return dirty;
    }
}