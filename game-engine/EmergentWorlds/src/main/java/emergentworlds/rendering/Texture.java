package emergentworlds.rendering;

import org.joml.Vector2f;
import org.lwjgl.BufferUtils;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.stb.STBImage.*;

public class Texture {
    private String filepath;
    private final int texID;

    private int width, height;

    public static final Vector2f[] DEFAULT_TEX_COORDS = new Vector2f[]{
            new Vector2f(1, 1), // top right
            new Vector2f(1, 0), // bottom right
            new Vector2f(0, 0), // bottom left
            new Vector2f(0, 1), // top left
    };

    public Texture(String filepath) {
        this.filepath = filepath;

        // --- Generate texture on the GPU
        texID = glGenTextures();
        glBindTexture(GL_TEXTURE_2D, texID);

        // --- Set texture parameters
        // Repeat image in both directions
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // pixelate when stretching, don't blur
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // pixelate when shrinking
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // --- Load the image
        IntBuffer width = BufferUtils.createIntBuffer(1);
        IntBuffer height = BufferUtils.createIntBuffer(1);
        IntBuffer channels = BufferUtils.createIntBuffer(1);

        stbi_set_flip_vertically_on_load(true);
        ByteBuffer image = stbi_load(filepath, width, height, channels, 0);

        if (image != null) {
            this.width = width.get(0);
            this.height = height.get(0);

            if (channels.get(0) == 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width.get(0), height.get(0),
                        0, GL_RGB, GL_UNSIGNED_BYTE, image);
            } else if (channels.get(0) == 4) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width.get(0), height.get(0),
                        0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            } else {
                throw new RuntimeException("Error: (Texture) Unsupported number of channels '" + channels.get(0) + "'");
            }
        } else {
            throw new RuntimeException("Error: (Texture) Could not load image '" + filepath + "'");
        }

        // Release image data from memory
        stbi_image_free(image);
    }

    public int getWidth() { return width; }
    public int getHeight() { return height; }

    public void bind() {
        glBindTexture(GL_TEXTURE_2D, texID);
    }

    public void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    public boolean equals(Object o) {
        if (!(o instanceof Texture)) return false;

        Texture other = (Texture)o;
        return filepath.equals(other.filepath);
    }
}
