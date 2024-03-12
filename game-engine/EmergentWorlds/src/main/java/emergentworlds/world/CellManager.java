package emergentworlds.world;

import emergentworlds.world.cell.Cell;
import org.joml.Vector2i;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.TreeSet;

/**
 * Handles world access to cell chunks
 */
public class CellManager {
    private TreeSet<CellChunk> chunks = new TreeSet<>((a, b) -> {
        int y = Integer.compare(b.getWorldPosition().y, a.getWorldPosition().y);
        if (y != 0) return y;
        return Integer.compare(a.getWorldPosition().x, b.getWorldPosition().x);
    });

    private int chunkWidth, chunkHeight;

    public CellManager(int chunkWidth, int chunkHeight) {
        this.chunkWidth = chunkWidth;
        this.chunkHeight = chunkHeight;
    }

    /**
     * @param position world coordinate
     * @return the CellChunk containing the global coordinate. Returns null
     *         if no chunk contains the coordinate.
     */
    public CellChunk getChunkContainingPosition(Vector2i position) {
        for (CellChunk chunk : this.chunks) {
            if (chunk.containsWorldCoordinate(position)) return chunk;
        }

        return null;
    }

    public Cell getCell(Vector2i position) {
        CellChunk chunk = getChunkContainingPosition(position);
        return chunk.getCellAtWorldPosition(position);
    }

    public void setCell(Vector2i position, Cell cell) {
        CellChunk chunk = getChunkContainingPosition(position);
        if (chunk == null) {
            Vector2i chunkPosition = new Vector2i(Math.floorDiv(position.x, chunkWidth)*chunkWidth, Math.floorDiv(position.y, chunkHeight)*chunkHeight);
            chunk = new CellChunk(chunkPosition, chunkWidth, chunkHeight);
            chunks.add(chunk);
        }
        chunk.setCellAtGlobalPosition(position, cell);
    }

    /**
     *
     * @return an Iterator across all cells managed by this CellManager across all cell chunks. Iterates through
     * each chunk one at a time.
     */
    public Iterator<Cell> cellIterator() {
        return new Iterator<Cell>() {
            final Iterator<CellChunk> chunksIterator = chunks.iterator();
            Iterator<Cell> currentChunk = null;

            @Override
            public boolean hasNext() {
                if (currentChunk != null && currentChunk.hasNext()) return true;
                else return chunksIterator.hasNext();
            }

            @Override
            public Cell next() {
                if (currentChunk == null) {
                    currentChunk = chunksIterator.next().iterator();
                }

                if (!currentChunk.hasNext()) {
                    currentChunk = chunksIterator.next().iterator();
                }

                return currentChunk.next();
            }
        };
    }
}
