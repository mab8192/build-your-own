package arkengine.components;

import arkengine.GameObject;

public abstract class Component {

    public GameObject gameObject;

    public void start() {}
    public void tick(double dt) {}
}
