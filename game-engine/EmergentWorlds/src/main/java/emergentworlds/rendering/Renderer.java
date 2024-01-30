package emergentworlds.rendering;

import emergentworlds.components.SpriteRenderer;
import emergentworlds.world.Entity;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class Renderer {
    private final int MAX_BATCH_SIZE = 1000;
    private final List<RenderBatch> renderBatches = new ArrayList<>();

    public Renderer() {

    }

    /**
     * Submit a game object to be included in a render pass
     * @param obj The object to be drawn. Should have a SpriteRenderer component.
     */
    public void submit(Entity obj) {
        ArrayList<SpriteRenderer> sprs = obj.getAllComponents(SpriteRenderer.class);
        for (SpriteRenderer spr : sprs) {
            add(spr);
        }
    }

    private void add(SpriteRenderer spr) {
        boolean added = false;
        for (RenderBatch batch : renderBatches) {
            if (batch.hasRoom() && batch.getzIndex() == spr._entity.getzIndex()) {
                Texture tex = spr.getTexture();
                if (tex == null || (batch.hasTexture(tex) || batch.hasTextureRoom())) {
                    batch.addSprite(spr);
                    added = true;
                    break;
                }
            }
        }

        if (!added) {
            RenderBatch batch = new RenderBatch(MAX_BATCH_SIZE, spr._entity.getzIndex());
            batch.start();
            batch.addSprite(spr);

            renderBatches.add(batch);

            // Ensure z index of batches is in the correct order
            renderBatches.sort(Comparator.comparingInt(RenderBatch::getzIndex));
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

    public void dispose() { }
}
