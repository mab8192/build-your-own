package emergentworlds.util;

import static org.lwjgl.glfw.GLFW.glfwGetTime;

public class TimeKeeper {
    /** @return the amount of time that has passed since the engine was initialized */
    public static double getTime() {
        return glfwGetTime();
    }
}
