package arkengine;

import arkengine.events.KeyListener;
import arkengine.events.MouseListener;
import arkengine.scene.LevelEditorScene;
import arkengine.scene.LevelScene;
import arkengine.util.Time;
import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.opengl.GL;
import arkengine.scene.Scene;

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

    private static Scene currentScene;
    public float r, g, b, a;

    private Window() {
        this.width = 1920;
        this.height = 1080;
        this.title = "ArkEngine";

        this.r = 1;
        this.g = 0;
        this.b = 0;
        this.a = 1;
    }

    public static void changeScene(Scene.SceneType newScene) {
        switch (newScene) {
            case LEVEL_EDITOR:
                currentScene = new LevelEditorScene();
                //currentScene.init();
                break;
            case LEVEL:
                currentScene = new LevelScene();
                break;
            default:
                throw new IllegalArgumentException("Unknown scene!");
        }
    }

    public static Window get() {
        if (Window.window == null) {
            Window.window = new Window();
        }

        return Window.window;
    }

    public void run() {
        init();
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

        // Create the window
        glfwWindow = glfwCreateWindow(this.width, this.height, this.title, NULL, NULL);
        if (glfwWindow == NULL) {
            throw new IllegalStateException("Failed to create the GLFW window!");
        }

        // Add callbacks
        glfwSetCursorPosCallback(glfwWindow, MouseListener::mousePosCallback);
        glfwSetMouseButtonCallback(glfwWindow, MouseListener::mouseButtonCallback);
        glfwSetScrollCallback(glfwWindow, MouseListener::mouseScrollCallback);
        glfwSetKeyCallback(glfwWindow, KeyListener::keyCallback);

        // Make the OpenGL context current
        glfwMakeContextCurrent(glfwWindow);

        // Enable vsync
        glfwSwapInterval(1);

        // Show the window
        glfwShowWindow(glfwWindow);

        // Required for LWJGL
        GL.createCapabilities();

        Window.changeScene(Scene.SceneType.LEVEL_EDITOR);
    }

    // Main application loop
    public void loop() {
        double beginTime = Time.getTime();
        double endTime;
        double dt = -1;

        while (!glfwWindowShouldClose(glfwWindow)) {
            glfwPollEvents();

            glClearColor(r, g, b, a);
            glClear(GL_COLOR_BUFFER_BIT); // Flush the set clear color to the entire screen

            // Update and render
            if (dt >= 0)
                currentScene.update(dt);

            glfwSwapBuffers(glfwWindow);

            endTime = Time.getTime();
            dt = endTime - beginTime;
            beginTime = endTime;
        }
    }
}
