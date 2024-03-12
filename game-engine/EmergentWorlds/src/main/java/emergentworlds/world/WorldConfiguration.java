package emergentworlds.world;

public class WorldConfiguration {
    // Defaults
    private int cellSize = 16;
    private int chunkWidth = 64;
    private int chunkHeight = 64;

    public WorldConfiguration cellSize(int cellSize) {
        this.cellSize = cellSize;
        return this;
    }

    public int cellSize() { return this.cellSize; }

    public WorldConfiguration chunkWidth(int chunkWidth) {
        this.chunkWidth = chunkWidth;
        return this;
    }

    public int chunkWidth() { return this.chunkWidth; }

    public WorldConfiguration chunkHeight(int chunkHeight) {
        this.chunkHeight = chunkHeight;
        return this;
    }

    public int chunkHeight() { return this.chunkHeight; }

    static WorldConfiguration DEFAULT = new WorldConfiguration();
}