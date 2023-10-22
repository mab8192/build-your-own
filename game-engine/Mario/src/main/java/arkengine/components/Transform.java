package arkengine.components;

import org.joml.Vector2f;

public class Transform extends Component{
    public Vector2f position = new Vector2f();
    public Vector2f scale = new Vector2f();
    public float rotation = 0.0f;

    public Transform() { }

    public Transform(Vector2f position) {
        this.position = position;
    }

    public Transform(Vector2f position, Vector2f scale) {
        this(position);
        this.scale = scale;
    }

    public Transform(Vector2f position, Vector2f scale, float rotation) {
        this(position, scale);
        this.rotation = rotation;
    }
}
