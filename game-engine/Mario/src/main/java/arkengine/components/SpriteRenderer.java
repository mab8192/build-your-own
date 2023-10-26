package arkengine.components;

import arkengine.rendering.Sprite;
import arkengine.rendering.Texture;
import arkengine.util.Color;
import org.joml.Vector2f;
import org.joml.Vector4f;

public class SpriteRenderer extends Component {
    private Sprite sprite;
    private boolean dirty = true; // whether the data should be reloaded for rendering
    private Transform lastTransform; // To keep track of our GameObject's transform for if that changes
    private Vector2f offset; // Offset at which to render the sprite from the parent gameobject's position

    public SpriteRenderer() {
        this(new Vector2f());
    }

    public SpriteRenderer(Vector2f offset) {
        this.sprite = new Sprite();
        this.offset = offset;
    }

    public SpriteRenderer(Vector4f color) {
        this(color, new Vector2f());
    }

    public SpriteRenderer(Vector4f color, Vector2f offset) {
        this.sprite = new Sprite(color);
        this.offset = offset;
    }

    public SpriteRenderer(Sprite sprite) {
        this(sprite, new Vector2f());
    }

    public SpriteRenderer(Sprite sprite, Vector2f offset) {
        this.sprite = sprite;
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

    public void setOffset(Vector2f offset) { this.offset = offset; dirty = true; }
    public Vector2f getOffset() { return this.offset; }

    public Vector4f getColor() {
        if (sprite != null)
            return sprite.getColor();
        return Color.WHITE;
    }

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

    public void setSprite(Sprite sprite) {
        this.sprite = sprite;
        dirty = true;
    }

    public void setClean() { dirty = false; }
    public boolean isDirty() { return dirty; }
}
