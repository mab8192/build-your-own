package arkengine.events;

import static org.lwjgl.glfw.GLFW.GLFW_PRESS;
import static org.lwjgl.glfw.GLFW.GLFW_RELEASE;

public class MouseListener {
    private static MouseListener instance;
    private double scrollX, scrollY;
    private double xpos, ypos, lastX, lastY;
    private boolean mouseButtonPressed[] = new boolean[3];
    private boolean isDragging;

    private MouseListener() {
        this.scrollX = 0.0;
        this.scrollY = 0.0;
        this.xpos = 0.0;
        this.ypos = 0.0;
        this.lastX = 0.0;
        this.lastY = 0.0;
    }

    public static MouseListener get() {
        if (MouseListener.instance == null) {
            MouseListener.instance = new MouseListener();
        }

        return MouseListener.instance;
    }

    public static void mousePosCallback(long window, double xpos, double ypos) {
        get().lastX = get().xpos;
        get().lastY = get().ypos;

        get().xpos = xpos;
        get().ypos = ypos;

        get().isDragging = get().mouseButtonPressed[0] || get().mouseButtonPressed[1] || get().mouseButtonPressed[2];
    }

    public static void mouseButtonCallback(long window, int button, int action, int mods) {
        if (button >= get().mouseButtonPressed.length) return;

        if (action == GLFW_PRESS)
            get().mouseButtonPressed[button] = true;
        else if (action == GLFW_RELEASE) {
            get().mouseButtonPressed[button] = false;
        }

    }

    public static void mouseScrollCallback(long window, double xOffset, double yOffset) {
        get().scrollX = xOffset;
        get().scrollY = yOffset;
    }

    public static void endFrame() {
        get().scrollX = 0;
        get().scrollY = 0;

        get().lastX = get().xpos;
        get().lastY = get().ypos;
    }

    public static float getX() {
        return (float)get().xpos;
    }

    public static float getY() {
        return (float)get().ypos;
    }

    public static float getDx() {
        return (float)(get().lastX - get().xpos);
    }

    public static float getDy() {
        return (float)(get().lastY - get().ypos);
    }

    public static float getScrollX() {
        return (float)get().scrollX;
    }

    public static float getScrollY() {
        return (float)get().scrollY;
    }

    public static boolean isDragging() {
        return get().isDragging;
    }

    public static boolean mouseButtonDown(int button) {
        if (button >= get().mouseButtonPressed.length)
            return false;
        return get().mouseButtonPressed[button];
    }
}
