package arkengine.components;

import arkengine.rendering.Sprite;
import arkengine.rendering.Texture;
import arkengine.util.Color;
import org.joml.Vector2f;
import org.joml.Vector4f;

public class SpriteRenderer extends Component {
    private Sprite sprite;
    private Vector4f color;
    private Vector2f offset; // Offset at which to render the sprite from the parent gameobject's position

    private boolean dirty = true; // whether the data should be reloaded for rendering
    private Transform lastTransform; // To keep track of our GameObject's transform for if that changes

    public SpriteRenderer() { this(new Sprite(), Color.WHITE, new Vector2f()); }
    public SpriteRenderer(Sprite sprite) { this(sprite, Color.WHITE, new Vector2f()); }
    public SpriteRenderer(Vector2f offset) { this(new Sprite(), Color.WHITE, offset); }
    public SpriteRenderer(Vector4f color) { this(new Sprite(), color, new Vector2f()); }
    public SpriteRenderer(Sprite sprite, Vector4f color) { this(sprite, color, new Vector2f()); }
    public SpriteRenderer(Vector4f color, Vector2f offset) { }
    public SpriteRenderer(Sprite sprite, Vector2f offset) { this(sprite, Color.WHITE, new Vector2f()); }
    public SpriteRenderer(Sprite sprite, Vector4f color, Vector2f offset) {
        this.sprite = sprite;
        this.color = color;
        this.offset = offset;
    }

    @Override
    public void start() {
        this.lastTransform = gameObject.transform.copy();
    }

    @Override
    public void tick(double dt) {
        // Check if transform or sprite have changed
        if (!lastTransform.equals(gameObject.transform)) {
            dirty = true;
            gameObject.transform.copyTo(lastTransform);
        }
    }

    public Vector2f getOffset() { return this.offset; }
    public void setOffset(Vector2f offset) { this.offset = offset; dirty = true; }

    public Vector4f getColor() { return color; }
    public void setColor(Vector4f color) { this.color = color; dirty = true; }
    public void setColor(float[] color) { this.color.set(color); dirty = true; }

    public Texture getTexture() {
        if (sprite != null)
            return sprite.getTexture();
        return null;
    }

    public Vector2f[] getTexCoords() {
        if (sprite != null)
            return sprite.getTexCoords();
        return Texture.DEFAULT_TEX_COORDS;
    }

    public Sprite getSprite() { return sprite; }

    public void setSprite(Sprite sprite) {
        this.sprite = sprite;
        dirty = true;
    }

    public void setClean() { dirty = false; }
    public void setDirty() { dirty = true; }
    public boolean isDirty() { return dirty; }
}
