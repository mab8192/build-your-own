package emergentworlds.components;

import emergentworlds.rendering.IRenderable;
import emergentworlds.rendering.Sprite;
import emergentworlds.rendering.Texture;
import emergentworlds.util.Color;
import org.joml.Vector2f;
import org.joml.Vector4f;

public class SpriteRenderer extends Component implements IRenderable {
    private Sprite _sprite;
    private Vector4f _color;
    private Vector2f offset; // Offset at which to render the sprite from the parent entity's position

    private boolean dirty = true;
    private Transform lastTransform; // To keep track of our Entity's transform for if that changes

    public SpriteRenderer() {
        this(new Sprite(), Color.WHITE, new Vector2f());
    }

    public SpriteRenderer(Sprite sprite) {
        this(sprite, Color.WHITE, new Vector2f());
    }

    public SpriteRenderer(Vector2f offset) {
        this(new Sprite(), Color.WHITE, offset);
    }

    public SpriteRenderer(Vector4f color) {
        this(new Sprite(), color, new Vector2f());
    }

    public SpriteRenderer(Sprite sprite, Vector4f color) {
        this(sprite, color, new Vector2f());
    }

    public SpriteRenderer(Vector4f color, Vector2f offset) {
    }

    public SpriteRenderer(Sprite sprite, Vector2f offset) {
        this(sprite, Color.WHITE, new Vector2f());
    }

    public SpriteRenderer(Sprite sprite, Vector4f color, Vector2f offset) {
        this._sprite = sprite;
        this._color = color;
        this.offset = offset;
    }

    @Override
    public void start() {
        this.lastTransform = _entity.transform.copy();
    }

    @Override
    public void tick(double dt) {
        // Check if transform or sprite have changed
        if (!lastTransform.equals(_entity.transform)) {
            setDirty();
            _entity.transform.copyTo(lastTransform);
        }
    }

    @Override
    public Vector2f getWorldPosition() {
        return _entity.transform.position;
    }

    @Override
    public Vector2f getOffset() {
        return this.offset;
    }

    public void setOffset(Vector2f offset) {
        this.offset = offset;
        setDirty();
    }

    @Override
    public Vector4f getColor() {
        return _color;
    }

    @Override
    public Vector2f getScale() {
        return _entity.transform.scale;
    }

    public void setColor(Vector4f _color) {
        this._color = _color;
        setDirty();
    }

    public void setColor(float[] color) {
        this._color.set(color);
        setDirty();
    }

    public Texture getTexture() {
        if (_sprite != null)
            return _sprite.getTexture();
        return null;
    }

    @Override
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

    @Override
    public void setClean() {
        dirty = false;
    }

    public void setDirty() {
        dirty = true;
    }

    @Override
    public boolean isDirty() {
        return dirty;
    }
}
