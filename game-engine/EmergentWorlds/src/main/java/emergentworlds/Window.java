package emergentworlds;

import emergentworlds.events.Input;
import emergentworlds.rendering.Renderer;
import emergentworlds.util.TimeKeeper;
import emergentworlds.world.World;
import org.joml.Vector3f;
import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.opengl.GL;

import static org.lwjgl.glfw.Callbacks.glfwFreeCallbacks;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.system.MemoryUtil.NULL;

public class Window {
    private int width, height;
    private String title;
    private long glfwWindow;

    // Singleton class
    private static Window window = null;

    private static World activeWorld;
    private static Renderer renderer;

    private Window() {
        this.width = 1920;
        this.height = 1080;
        this.title = "ArkEngine";
    }

    public static void loadWorld(World world) {
        renderer.reset();

        activeWorld = world;

        activeWorld.init();
        activeWorld.start();
    }

    public static World getWorld() {
        return activeWorld;
    }

    public static Renderer getRenderer() { return renderer; }

    public static Window get() {
        if (Window.window == null) {
            Window.window = new Window();
        }

        return Window.window;
    }

    public void run() {
        loop();

        // Free memory
        glfwFreeCallbacks(glfwWindow);
        glfwDestroyWindow(glfwWindow);

        // Terminate GLFW and free error callback
        glfwTerminate();
        glfwSetErrorCallback(null).free();
    }

    public void init() {
        GLFWErrorCallback.createPrint(System.err).set();

        // Init GLFW
        if (!glfwInit()) {
            throw new IllegalStateException("Unable to initialize GLFW!");
        }

        // Set up GLFW
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // keep window invisible until done creating
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

        // Using 3.3 so we can set glslVersion for ImGui as #version 330
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        // Create the window
        glfwWindow = glfwCreateWindow(this.width, this.height, this.title, NULL, NULL);
        if (glfwWindow == NULL) {
            throw new IllegalStateException("Failed to create the GLFW window!");
        }

        // Add callbacks
        glfwSetCursorPosCallback(glfwWindow, Input::mousePosCallback);
        glfwSetMouseButtonCallback(glfwWindow, Input::mouseButtonCallback);
        glfwSetScrollCallback(glfwWindow, Input::mouseScrollCallback);
        glfwSetKeyCallback(glfwWindow, Input::keyCallback);

        // Make the OpenGL context current
        glfwMakeContextCurrent(glfwWindow);

        // Enable vsync
        glfwSwapInterval(1);

        // Show the window
        glfwShowWindow(glfwWindow);

        // Required for LWJGL
        GL.createCapabilities();

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        renderer = new Renderer();
    }

    // Main application loop
    public void loop() {
        double beginTime = TimeKeeper.getTime();
        double endTime;
        double dt = -1;

        while (!glfwWindowShouldClose(glfwWindow)) {
            glfwPollEvents();

            Vector3f clearColor = activeWorld.getCamera().getClearColor();
            glClearColor(clearColor.x, clearColor.y, clearColor.z, 1);
            glClear(GL_COLOR_BUFFER_BIT); // Flush the set clear color to the entire screen

            // Update and render
            if (dt >= 0) {
                activeWorld.tick(dt);
                renderer.render();
            }

            glfwSwapBuffers(glfwWindow);

            // Clear input buffers
            Input.endFrame();

            endTime = TimeKeeper.getTime();
            dt = endTime - beginTime;
            beginTime = endTime;
        }
    }

    public void destroy() {
        glfwTerminate();
    }
}
