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

    public SpriteRenderer() {
        this.sprite = new Sprite();
    }

    public SpriteRenderer(Vector4f color) {
        this.sprite = new Sprite(color);
    }

    public SpriteRenderer(Sprite sprite) {
        this.sprite = sprite;
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
