package emergentworlds.util;

import emergentworlds.rendering.Shader;
import emergentworlds.rendering.Spritesheet;
import emergentworlds.rendering.Texture;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

/**
 * Tracks objects by filepath.
 * Useful for maintaining references to things that helps prevent garbage collection
 * from causing frame drops.
 */
public class AssetPool {
    private static final Map<String, Shader> shaders = new HashMap<>();
    private static final Map<String, Texture> textures = new HashMap<>();
    private static final Map<String, Spritesheet> spritesheets = new HashMap<>();

    public static Shader getShader(String filepath) {
        File file = new File(filepath);

        if (shaders.containsKey(file.getAbsolutePath()))
            return shaders.get(file.getAbsolutePath());

        Shader shader = new Shader(filepath);
        shader.compile();
        shaders.put(file.getAbsolutePath(), shader);
        return shader;
    }

    public static Texture getTexture(String filepath) {
        File file = new File(filepath);

        if (textures.containsKey(file.getAbsolutePath()))
            return textures.get(file.getAbsolutePath());

        Texture texture = new Texture(filepath);
        textures.put(file.getAbsolutePath(), texture);
        return texture;
    }

    public static void putSpritesheet(String filepath, Spritesheet spritesheet) {
        File file = new File(filepath);
        spritesheets.put(file.getAbsolutePath(), spritesheet);
    }

    public static Spritesheet getSpritesheet(String filepath) {
        File file = new File(filepath);

        if (!spritesheets.containsKey(file.getAbsolutePath()))
            throw new RuntimeException("Attempted to get spritesheet: " + filepath + " before it was loaded.");

        return spritesheets.get(file.getAbsolutePath());
    }
}
