package arkengine.components;

import arkengine.rendering.Texture;
import arkengine.util.Color;
import org.joml.Vector2f;
import org.joml.Vector4f;

public class SpriteRenderer extends Component {
    private Sprite sprite; // a reference to the sprite component of the attached GameObject
    private boolean dirty = true; // whether the data should be reloaded for rendering
    private Sprite lastSprite; // To keep track of the Sprite component in case it changes
    private Transform lastTransform; // To keep track of our GameObject's transform for if that changes

    public SpriteRenderer() {

    }

    public void start() {
        this.sprite = gameObject.getComponent(Sprite.class); // could be null
        this.lastSprite = this.sprite;
        this.lastTransform = gameObject.transform.copy();
    }

    public void tick(double dt) {
        // Check if transform or sprite have changed
        if (!lastTransform.equals(gameObject.transform)) {
            dirty = true;
            gameObject.transform.copyTo(lastTransform);
        }

        Sprite sprite = gameObject.getComponent(Sprite.class);
        if (!lastSprite.equals(sprite)) {
            dirty = true;
            sprite.copyTo(lastSprite);
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

    public void setClean() { dirty = false; }
    public boolean isDirty() { return dirty; }
}
