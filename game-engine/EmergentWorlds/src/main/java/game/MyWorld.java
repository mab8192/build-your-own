package game;

import emergentworlds.events.Input;
import emergentworlds.world.World;
import emergentworlds.world.cell.cells.DirtCell;
import emergentworlds.world.cell.cells.VoidCell;
import org.joml.Vector2f;
import org.joml.Vector3f;

import java.awt.event.KeyEvent;

public class MyWorld extends World {

    float cameraSpeed = 100f;

    public void init() {
        setCellSize(4);
        setWorldSize(500, 100);

        for (int i = 0; i < _worldWidth; i++) {
            for (int j = 0; j < _worldHeight; j++) {
                setCell(i, j, new DirtCell(this));
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
            _camera.position.add(movement.normalize().mul((float) dt * cameraSpeed));
        }
    }
}
