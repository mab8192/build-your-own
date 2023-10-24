package arkengine;

import arkengine.events.Input;
import arkengine.rendering.Renderer;
import arkengine.scene.LevelEditorScene;
import arkengine.scene.LevelScene;
import arkengine.util.Time;
import org.joml.Vector3f;
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
    private static Renderer renderer = new Renderer();

    private Window() {
        this.width = 1920;
        this.height = 1080;
        this.title = "ArkEngine";
    }

    public static void changeScene(Scene.SceneType newScene) {
        switch (newScene) {
            case LEVEL_EDITOR:
                currentScene = new LevelEditorScene();
                break;
            case LEVEL:
                currentScene = new LevelScene();
                break;
            default:
                throw new IllegalArgumentException("Unknown scene!");
        }

        currentScene.init();
        currentScene.start();
    }

    public static Scene getScene() {
        return get().currentScene;
    }

    public static Renderer getRenderer() { return get().renderer; }

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

        Window.changeScene(Scene.SceneType.LEVEL_EDITOR);
    }

    // Main application loop
    public void loop() {
        double beginTime = Time.getTime();
        double endTime;
        double dt = -1;

        while (!glfwWindowShouldClose(glfwWindow)) {
            glfwPollEvents();

            Vector3f clearColor = currentScene.getCamera().getClearColor();
            glClearColor(clearColor.x, clearColor.y, clearColor.z, 1);
            glClear(GL_COLOR_BUFFER_BIT); // Flush the set clear color to the entire screen

            // Update and render
            if (dt >= 0) {
                currentScene.tick(dt);
                renderer.render();
            }
            
            glfwSwapBuffers(glfwWindow);

            // Clear input buffers
            Input.endFrame();

            endTime = Time.getTime();
            dt = endTime - beginTime;
            beginTime = endTime;
        }
    }
}
