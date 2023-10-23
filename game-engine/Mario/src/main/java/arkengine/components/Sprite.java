package arkengine.components;

import arkengine.rendering.Texture;
import arkengine.util.Color;
import org.joml.Vector2f;
import org.joml.Vector4f;

import java.util.Arrays;

public class Sprite extends Component{

    private Texture texture;
    private Vector4f color;
    private Vector2f[] texCoords;

    public Sprite() {
        this.texture = null;
        this.texCoords = Texture.DEFAULT_TEX_COORDS;
        this.color = Color.WHITE;
    }

    public Sprite(Vector4f color) {
        this.texture = null;
        this.texCoords = Texture.DEFAULT_TEX_COORDS;
        this.color = color;
    }

    public Sprite(Texture tex) {
        this.texture = tex;
        this.texCoords = Texture.DEFAULT_TEX_COORDS;
        this.color = Color.WHITE;
    }

    public Sprite(Texture tex, Vector2f[] texCoords) {
        this.texture = tex;
        this.texCoords = texCoords;
        this.color = Color.WHITE;
    }

    public Vector4f getColor() {
        return color;
    }

    public Texture getTexture() {
        return texture;
    }

    public Vector2f[] getTexCoords() { return texCoords; }

    public void copyTo(Sprite to) {
        to.texture = this.texture;
        to.color = this.color;
        to.texCoords = this.texCoords;
    }

    public boolean equals(Object o) {
        if (!(o instanceof Sprite)) return false;

        Sprite other = (Sprite)o;
        if ((texture != null && other.texture == null) || (texture == null && other.texture != null)) return false;
        return (texture == null || texture.equals(other.texture)) && color.equals(other.color) && Arrays.equals(texCoords, other.texCoords);
    }
}
