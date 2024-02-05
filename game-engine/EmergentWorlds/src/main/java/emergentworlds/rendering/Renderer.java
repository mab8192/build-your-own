package emergentworlds.rendering;

import emergentworlds.components.SpriteRenderer;
import emergentworlds.world.Entity;
import emergentworlds.world.cell.Cell;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

import static emergentworlds.rendering.CellRenderer.CELL_Z_INDEX;

public class Renderer {
    private final int MAX_BATCH_SIZE = 1000;
    private List<RenderBatch> renderBatches = new ArrayList<>();

    public Renderer() { }

    /**
     * Submit a game object to be included in a render pass
     * @param obj The object to be drawn. Should have a SpriteRenderer component.
     */
    public void submit(Object obj) {
        if (obj instanceof Entity ent) {
            ArrayList<SpriteRenderer> sprs = ent.getAllComponents(SpriteRenderer.class);
            for (SpriteRenderer spr : sprs) {
                add(spr);
            }
        } else if (obj instanceof Cell cell) {
            add(cell.getRenderer());
        } else {
            throw new UnsupportedOperationException("Cannot submit object of type: " + obj.getClass() + " for rendering.");
        }
    }

    private void add(IRenderable spr) {
        boolean added = false;
        int zIndex = (spr instanceof SpriteRenderer sprite) ? sprite._entity.getzIndex() : CELL_Z_INDEX;
        for (RenderBatch batch : renderBatches) {
            if (batch.hasRoom() && batch.getzIndex() == zIndex) {
                Texture tex = spr.getTexture();
                if (tex == null || (batch.hasTexture(tex) || batch.hasTextureRoom())) {
                    batch.add(spr);
                    added = true;
                    break;
                }
            }
        }

        if (!added) {
            RenderBatch batch = new RenderBatch(MAX_BATCH_SIZE, zIndex);
            batch.start();
            batch.add(spr);

            renderBatches.add(batch);

            // Ensure z index of batches is in the correct order
            renderBatches.sort(Comparator.comparingInt(RenderBatch::getzIndex));
        }
    }

    public void remove(IRenderable spr) {
        for (RenderBatch batch : renderBatches) {
            batch.remove(spr);
        }
    }

    /**
     * Render previously submitted draw calls.
     */
    public void render() {
        // Draw the submitted objects to the screen
        for (RenderBatch batch : renderBatches) {
            batch.render();
        }
    }

    public void reset() {
        // Clears render batches and resets to a new state
        renderBatches = new ArrayList<>();
    }

    public void dispose() { }
}
