package emergentworlds.events;

import java.awt.event.KeyEvent;
import java.util.HashMap;
import java.util.Map;

import static org.lwjgl.glfw.GLFW.*;

public class Input {
    private static Input instance;

    public Map<String, int[]> keyMap = new HashMap<>();
    private final boolean[] keyPressed = new boolean[350];

    private double scrollX = 0, scrollY = 0;
    private double xpos = 0, ypos = 0, lastX = 0, lastY = 0;
    private final boolean[] mouseButtonPressed = new boolean[3];
    private boolean isMouseDragging;

    private Input() {
        keyMap.put("UP", new int[]{KeyEvent.VK_W, KeyEvent.VK_UP});
        keyMap.put("LEFT", new int[]{KeyEvent.VK_A, KeyEvent.VK_LEFT});
        keyMap.put("DOWN", new int[]{KeyEvent.VK_S, KeyEvent.VK_DOWN});
        keyMap.put("RIGHT", new int[]{KeyEvent.VK_D, KeyEvent.VK_RIGHT});
        keyMap.put("JUMP", new int[]{KeyEvent.VK_SPACE});
    }

    public static Input get() {
        if (instance == null)
            instance = new Input();
        return instance;
    }


    // ----- API level stuff
    public static boolean isButtonDown(String name) {
        String norm = name.toUpperCase();

        if (!get().keyMap.containsKey(norm)) return false;

        int[] keys = get().keyMap.get(norm);
        for (int i : keys) {
            if (Input.isKeyPressed(i)) return true;
        }

        return false;
    }

    public static float getAxis(String name) {
        String norm = name.toUpperCase();

        float result = 0.0f;

        if (norm.equals("HORIZONTAL")) {
            // check for left/right axis movement
            if (isButtonDown("right")) result += 1.0f;
            if (isButtonDown("left")) result -= 1.0f;
        } else if (norm.equals("VERTICAL")) {
            // Check for up/down axis movement
            if (isButtonDown("up")) result += 1.0f;
            if (isButtonDown("down")) result -= 1.0f;
        } else {
            throw new RuntimeException("Invalid axis: " + name);
        }

        // return result clamped to [-1, 1]
        return Math.min(Math.max(result, 0.0f), 1.0f);
    }

    // ----- Keyboard stuff
    public static boolean isKeyPressed(int keyCode) {
        if (keyCode >= get().keyPressed.length)
            throw new ArrayIndexOutOfBoundsException("Key code out of bounds!");

        return get().keyPressed[keyCode];
    }

    // ----- Mouse Listener stuff
    public static void endFrame() {
        get().scrollX = 0;
        get().scrollY = 0;

        get().lastX = get().xpos;
        get().lastY = get().ypos;
    }

    public static float getMouseX() {
        return (float)get().xpos;
    }

    public static float getMouseY() {
        return (float)get().ypos;
    }

    public static float getMouseDx() {
        return (float)(get().lastX - get().xpos);
    }

    public static float getMouseDy() {
        return (float)(get().lastY - get().ypos);
    }

    public static float getMouseScrollX() {
        return (float)get().scrollX;
    }

    public static float getMouseScrollY() {
        return (float)get().scrollY;
    }

    public static boolean isMouseDragging() {
        return get().isMouseDragging;
    }

    public static boolean mouseButtonDown(int button) {
        if (button >= get().mouseButtonPressed.length)
            return false;
        return get().mouseButtonPressed[button];
    }

    // ----- Controller stuff (TODO)

    // ----- GLFW Callbacks

    public static void keyCallback(long window, int key, int scancode, int action, int mods) {
        if (key >= get().keyPressed.length)
            return;

        if (action == GLFW_PRESS) {
            get().keyPressed[key] = true;
        } else if (action == GLFW_RELEASE) {
            get().keyPressed[key] = false;
        }
    }

    public static void mousePosCallback(long window, double xpos, double ypos) {
        get().lastX = get().xpos;
        get().lastY = get().ypos;

        get().xpos = xpos;
        get().ypos = ypos;

        get().isMouseDragging = get().mouseButtonPressed[0] || get().mouseButtonPressed[1] || get().mouseButtonPressed[2];
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
}
