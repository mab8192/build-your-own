package arkengine.components;

import arkengine.rendering.Spritesheet;
import arkengine.rendering.Texture;
import org.joml.Vector2f;
import org.joml.Vector4f;

public class Sprite extends Component{

    private Texture texture;
    private Vector4f color;
    private Vector2f[] texCoords;

    private final Vector2f[] DEFAULT_TEX_COORDS = new Vector2f[]{
            new Vector2f(1, 1), // top right
            new Vector2f(1, 0), // bottom right
            new Vector2f(0, 0), // bottom left
            new Vector2f(0, 1), // top left
    };

    private final Vector4f COLOR_WHITE = new Vector4f(1, 1, 1, 1);

    public Sprite() {
        this.texture = null;
        this.texCoords = DEFAULT_TEX_COORDS;
        this.color = COLOR_WHITE;
    }

    public Sprite(Vector4f color) {
        this.texture = null;
        this.texCoords = DEFAULT_TEX_COORDS;
        this.color = color;
    }

    public Sprite(Texture tex) {
        this.texture = tex;
        this.texCoords = DEFAULT_TEX_COORDS;
        this.color = COLOR_WHITE;
    }

    public Sprite(Spritesheet spritesheet, int row, int col) {
        this.texture = spritesheet.getTexture();
        this.texCoords = spritesheet.getTexCoords(row, col);
        this.color = COLOR_WHITE;
    }

    public Vector4f getColor() {
        return color;
    }

    public Texture getTexture() {
        return texture;
    }

    public Vector2f[] getTexCoords() { return texCoords; }
}
