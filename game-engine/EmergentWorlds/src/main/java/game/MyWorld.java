package game;

import emergentworlds.events.Input;
import emergentworlds.world.World;
import emergentworlds.world.WorldConfiguration;
import emergentworlds.world.cell.cells.DirtCell;
import org.joml.Vector2i;
import org.joml.Vector3f;

import java.awt.event.KeyEvent;

public class MyWorld extends World {

    float cameraSpeed = 100f;

    public MyWorld(WorldConfiguration config) {
        super(config);
    }

    public void init() {
        for (int y = 0; y < 64; y++) {
            for (int x = 0; x < 200; x++) {
                setCell(new Vector2i(x, y), new DirtCell());
            }
        }
    }

    public void tick(double dt) {
        super.tick(dt);

        Vector3f movement = new Vector3f(0, 0, 0);

        if (Input.isKeyPressed(KeyEvent.VK_W)) {
            movement.y += 1;
        }

        if (Input.isKeyPressed(KeyEvent.VK_A)) {
            movement.x -= 1;
        }

        if (Input.isKeyPressed(KeyEvent.VK_S)) {
            movement.y -= 1;
        }

        if (Input.isKeyPressed(KeyEvent.VK_D)) {
            movement.x += 1;
        }

        if (movement.length() > 0) {
            camera.position.add(movement.normalize().mul((float) dt * cameraSpeed));
        }
    }
}
