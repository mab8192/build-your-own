package arkengine.util;

import static org.lwjgl.glfw.GLFW.glfwGetTime;

public class Time {
    public static double getTime() {
        return glfwGetTime();
    }
}
