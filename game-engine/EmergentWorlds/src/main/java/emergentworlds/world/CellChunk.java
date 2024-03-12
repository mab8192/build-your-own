package emergentworlds.world;

import emergentworlds.world.cell.Cell;
import emergentworlds.world.cell.cells.VoidCell;
import org.joml.Vector2i;

import java.util.Arrays;
import java.util.Iterator;
import java.util.Objects;

/**
 * Chunks have their bottom-left corner as coordinate (0, 0)
 */
public class CellChunk {
    private final Vector2i worldPosition;
    private final int layer = 0; // TODO: Support chunk layering
    private final int width, height;
    private final Cell[] cells;

    public CellChunk(Vector2i worldPosition, int width, int height) {
        this.worldPosition = worldPosition;
        this.cells = new Cell[width * height];
        this.width = width;
        this.height = height;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                VoidCell cell = new VoidCell();
                this.cells[y * height + x] = cell;
            }
        }
    }

    @Override
    public int hashCode() {
        return Objects.hash(this.worldPosition, this.layer);
    }

    public Vector2i getWorldPosition() {
        return this.worldPosition;
    }

    public int getWidth() {
        return this.width;
    }

    public int getHeight() {
        return this.height;
    }

    public boolean containsWorldCoordinate(Vector2i position) {
        return position.x < worldPosition.x + width && position.x >= worldPosition.x && position.y < worldPosition.y + height && position.y >= worldPosition.y;
    }

    public Vector2i WorldCoordinateToLocal(Vector2i position) {
        return position.sub(worldPosition);
    }

    public Vector2i localCoordinateToWorld(Vector2i position) {
        return position.add(worldPosition);
    }

    public void setCellAtLocalPosition(Vector2i position, Cell cell) {
        cells[position.y * height + position.x] = cell;
        cell.setWorldPosition(localCoordinateToWorld(position));
    }

    public void setCellAtGlobalPosition(Vector2i position, Cell cell) {
        if (!containsWorldCoordinate(position))
            throw new IllegalArgumentException("Coordinate (" + position.x + ", " + position.y + ") outside of chunk");
        setCellAtLocalPosition(WorldCoordinateToLocal(position), cell);
    }

    public Cell getCellAtLocalPosition(Vector2i position) {
        int index = position.y * height + position.x;
        return cells[index];
    }

    public Cell getCellAtWorldPosition(Vector2i position) {
        if (!containsWorldCoordinate(position))
            throw new IllegalArgumentException("Coordinate " + position + " outside of chunk");
        Vector2i local = WorldCoordinateToLocal(position);
        return getCellAtLocalPosition(local);
    }

    public Iterator<Cell> iterator() {
        return Arrays.stream(cells).iterator();
    }
}
