package arkengine.scene;

import arkengine.components.Component;
import arkengine.components.Transform;

import java.util.ArrayList;
import java.util.List;

public class GameObject {
    private String name;
    public Transform transform;
    private int zIndex = 0;
    private List<Component> components = new ArrayList<>();

    public GameObject(String name) {
        this(name, new Transform());
    }

    public GameObject(String name, Transform transform) {
        this(name, transform, 0);
    }

    public GameObject(String name, Transform transform, int zIndex) {
        this.name = name;
        this.transform = transform;
        this.zIndex = zIndex;
    }

    public GameObject withComponent(Component c) {
        addComponent(c);
        return this;
    }

    public void addComponent(Component c) {
        c.gameObject = this;
        components.add(c);
    }

    public void setName(String name) {
        this.name = name;
    }
    public String getName() { return name; }

    public int getzIndex() { return zIndex; }

    public <T extends Component> T getComponent(Class<T> componentClass) {
        for (Component c : components) {
            if (componentClass.isAssignableFrom(c.getClass())) {
                try {
                    return componentClass.cast(c);
                } catch (ClassCastException e) {
                    e.printStackTrace();
                    throw new RuntimeException("Failed to cast component");
                }
            }
        }

        return null;
    }

    public <T extends Component> ArrayList<T> getAllComponents(Class<T> componentClass) {
        ArrayList<T> requestedComponents = new ArrayList<>();
        for (Component c : components) {
            if (componentClass.isAssignableFrom(c.getClass())) {
                try {
                    requestedComponents.add(componentClass.cast(c));
                } catch (ClassCastException e) {
                    e.printStackTrace();
                    throw new RuntimeException("Failed to cast component");
                }
            }
        }

        return requestedComponents;
    }

    public <T extends Component> void removeComponent(Class<T> componentClass) {
        for (int i = 0; i < components.size(); i++) {
            Component c = components.get(i);
            if (componentClass.isAssignableFrom(c.getClass())) {
                components.remove(i);
                return;
            }
        }
    }

    public void start() {
        // Anything needed to start game objects should happen before component start calls
        // ...

        for (Component c : components) {
            c.start();
        }
    }

    public void tick(double dt) {
        for (Component c : components) {
            c.tick(dt);
        }
    }
}
