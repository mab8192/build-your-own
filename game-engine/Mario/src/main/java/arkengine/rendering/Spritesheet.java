package arkengine.rendering;

import org.joml.Vector2f;

import java.util.ArrayList;
import java.util.List;

public class Spritesheet {
    private final Texture texture;
    private final List<Vector2f[]> spriteTexCoords = new ArrayList<>();
    private final int numCols;

    public Spritesheet(Texture texture, int spriteWidth, int spriteHeight, int spacing) {
        this.texture = texture;
        this.numCols = texture.getWidth() / (spriteWidth + spacing);

        // Precompute texture coordinates
        int currentX = 0;
        int currentY = texture.getHeight() - spriteHeight;
        do {
            float topY = (currentY + spriteHeight) / (float) texture.getHeight();
            float rightX = (currentX + spriteWidth) / (float) texture.getWidth();
            float leftX = currentX / (float) texture.getWidth();
            float bottomY = currentY / (float) texture.getHeight();

            Vector2f[] texCoords = {
                    new Vector2f(rightX, topY),
                    new Vector2f(rightX, bottomY),
                    new Vector2f(leftX, bottomY),
                    new Vector2f(leftX, topY)
            };

            spriteTexCoords.add(texCoords);

            currentX += spriteWidth + spacing;
            if (currentX >= texture.getWidth()) {
                currentX = 0;
                currentY -= spriteHeight + spacing;
            }

        } while (currentY >= 0);
    }

    public Texture getTexture() {
        return texture;
    }

    /**
     * Get the texture coordinates for a sprite at the requested index. Top left is (0, 0).
     * @param row The row number of the sprite to get texCoords for.
     * @param col The col number of the sprite to get texCoords for.
     * @return a Vector2f[] containing the texture coordinates
     */
    public Vector2f[] getTexCoords(int row, int col) {
        int spriteIndex = row * numCols + col;

        if (spriteIndex >= spriteTexCoords.size()) {
            throw new RuntimeException("Cannot get sprite at row " + row + ", col " + col + ". Exceeds bounds.");
        }

        return spriteTexCoords.get(spriteIndex);
    }

    public Sprite getSprite(int row, int col) {
        return new Sprite(texture, getTexCoords(row, col));
    }

    public int getNumSprites() {
        return spriteTexCoords.size();
    }
}
