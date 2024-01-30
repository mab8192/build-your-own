package emergentworlds.components;

import emergentworlds.world.Entity;

public abstract class Component {

    public Entity _entity;

    public void start() {}
    public void tick(double dt) {}
}
