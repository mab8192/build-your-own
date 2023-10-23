package arkengine.components;

import org.joml.Vector2f;

public class Transform {
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

    public Transform copy() {
        return new Transform(new Vector2f(position), new Vector2f(scale), rotation);
    }

    public void copyTo(Transform to) {
        to.position.set(this.position);
        to.scale.set(this.scale);
        to.rotation = this.rotation;
    }

    public boolean equals(Object o) {
        if (!(o instanceof Transform)) return false;
        Transform t = (Transform)o;

        return t.position.equals(position) && t.scale.equals(scale) && t.rotation == rotation;
    }
}
