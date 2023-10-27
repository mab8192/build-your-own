package arkengine.rendering;

import arkengine.util.Color;
import org.joml.Vector2f;
import org.joml.Vector4f;

import java.util.Arrays;

public class Sprite {

    private Texture texture;
    private Vector2f[] texCoords;

    public Sprite() {
        this.texture = null;
        this.texCoords = Texture.DEFAULT_TEX_COORDS;
    }


    public Sprite(Texture tex) {
        this.texture = tex;
        this.texCoords = Texture.DEFAULT_TEX_COORDS;
    }

    public Sprite(Texture tex, Vector2f[] texCoords) {
        this.texture = tex;
        this.texCoords = texCoords;
    }

    public Texture getTexture() {
        return texture;
    }
    public Vector2f[] getTexCoords() { return texCoords; }
    public void setTexCoords(Vector2f[] texCoords) { this.texCoords = texCoords; }

    public void copyTo(Sprite to) {
        to.texture = this.texture;
        to.texCoords = this.texCoords;
    }

    public boolean equals(Object o) {
        if (!(o instanceof Sprite)) return false;

        Sprite other = (Sprite)o;
        if ((texture != null && other.texture == null) || (texture == null && other.texture != null)) return false;
        return (texture == null || texture.equals(other.texture)) && Arrays.equals(texCoords, other.texCoords);
    }
}
