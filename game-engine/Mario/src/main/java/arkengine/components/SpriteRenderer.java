package arkengine.components;

import org.joml.Vector4f;

public class SpriteRenderer extends Component {

    private Vector4f color;

    public SpriteRenderer(Vector4f color) {
        this.color = color;
    }

    @Override
    public void start() {

    }

    @Override
    public void tick(double dt) {

    }

    public Vector4f getColor() {
        return color;
    }
}
