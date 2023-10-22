package arkengine.rendering;

import arkengine.GameObject;
import arkengine.components.SpriteRenderer;

import java.util.ArrayList;
import java.util.List;

public class Renderer {
    private final int MAX_BATCH_SIZE = 1000;
    private List<RenderBatch> renderBatches = new ArrayList<>();

    public Renderer() { }

    public void submit(GameObject obj) {
        SpriteRenderer spr = obj.getComponent(SpriteRenderer.class);
        if (spr != null) {
            add(spr);
        }
    }

    private void add(SpriteRenderer spr) {
        boolean added = false;
        for (RenderBatch batch : renderBatches) {
            if (batch.hasRoom()) {
                batch.addSprite(spr);
                added = true;
                break;
            }
        }

        if (!added) {
            RenderBatch batch = new RenderBatch(MAX_BATCH_SIZE);
            batch.start();
            batch.addSprite(spr);

            renderBatches.add(batch);
        }
    }

    public void render() {
        // Draw the submitted objects to the screen
        for (RenderBatch batch : renderBatches) {
            batch.render();
        }
    }
}
