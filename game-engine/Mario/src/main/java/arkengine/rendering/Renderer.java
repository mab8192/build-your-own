package arkengine.rendering;

import arkengine.components.Sprite;
import arkengine.scene.GameObject;

import java.util.ArrayList;
import java.util.List;

public class Renderer {
    private final int MAX_BATCH_SIZE = 1000;
    private List<RenderBatch> renderBatches = new ArrayList<>();

    public Renderer() { }

    /**
     * Submit a game object to be included in a render pass
     * @param obj The object to be drawn. Should have a Sprite component.
     */
    public void submit(GameObject obj) {
        Sprite spr = obj.getComponent(Sprite.class);
        if (spr != null) {
            add(spr);
        }
    }

    private void add(Sprite spr) {
        boolean added = false;
        for (RenderBatch batch : renderBatches) {
            if (batch.hasRoom()) {
                Texture tex = spr.getTexture();
                if (tex == null || (batch.hasTexture(tex) || batch.hasTextureRoom())) {
                    batch.addSprite(spr);
                    added = true;
                    break;
                }
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
