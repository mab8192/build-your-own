package arkengine.components;

import arkengine.rendering.Texture;
import org.joml.Vector2f;
import org.joml.Vector4f;
import org.w3c.dom.Text;

public class SpriteRenderer extends Component {

    private Vector4f color;
    private Vector2f[] texCoords = new Vector2f[4]; // Each sprite is a quad, so 4 texture coordinates
    private Texture texture;

    public SpriteRenderer(Vector4f color) {
        this.texture = null;
        this.color = color;
    }
    public SpriteRenderer(Texture texture) {
        this.texture = texture;
        this.color = new Vector4f(1, 1, 1, 1);
    }

    @Override
    public void start() {

    }

    @Override
    public void tick(double dt) {

    }

    public Vector4f getColor() {
        return color;
    }

    public Texture getTexture() { return texture; }
    public Vector2f[] getTexCoords() {
        return new Vector2f[]{
                new Vector2f(1, 1), // top right
                new Vector2f(1, 0), // bottom right
                new Vector2f(0, 0), // bottom left
                new Vector2f(0, 1), // top left
        };
    }
}
