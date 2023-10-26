package arkengine.rendering;

import arkengine.scene.Scene;
import imgui.*;
import imgui.flag.ImGuiConfigFlags;
import imgui.flag.ImGuiFreeTypeBuilderFlags;
import imgui.gl3.ImGuiImplGl3;
import imgui.glfw.ImGuiImplGlfw;
import org.lwjgl.glfw.GLFW;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class ImGuiLayer {
    private final ImGuiImplGlfw imGuiGlfw = new ImGuiImplGlfw();
    private final ImGuiImplGl3 imGuiGl3 = new ImGuiImplGl3();

    public ImGuiLayer() { }

    private static byte[] loadFromResources(String filepath) {
        try {
            return Files.readAllBytes(Paths.get(filepath));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public void init(long glfwWindow) {
        ImGui.createContext();

        // Init ImGui config
        final ImGuiIO io = ImGui.getIO();
        io.setIniFilename("imgui.ini");                                // We don't want to save .ini file
        io.addConfigFlags(ImGuiConfigFlags.NavEnableKeyboard);  // Enable Keyboard Controls
        io.addConfigFlags(ImGuiConfigFlags.DockingEnable);      // Enable Docking
        io.addConfigFlags(ImGuiConfigFlags.ViewportsEnable);    // Enable multi-viewports
        io.setBackendPlatformName("imgui_java_impl_glfw");
        io.setConfigViewportsNoTaskBarIcon(true);

        // Set up fonts
        ImFontAtlas fontAtlas = io.getFonts();
        ImFontConfig fontConfig = new ImFontConfig();
        fontConfig.setGlyphRanges(fontAtlas.getGlyphRangesDefault());

        fontConfig.setPixelSnapH(true);
        fontAtlas.addFontFromFileTTF("assets/fonts/VCR_OSD_MONO.ttf", 18, fontConfig);
        fontAtlas.addFontDefault();

        fontConfig.destroy();
        fontAtlas.setFlags(ImGuiFreeTypeBuilderFlags.LightHinting);
        fontAtlas.build();

        // Init rendering
        imGuiGlfw.init(glfwWindow, true);
        imGuiGl3.init("#version 330 core");

        // Styling
        ImGui.styleColorsClassic();
    }

    public void newFrame() {
        imGuiGlfw.newFrame();
        ImGui.newFrame();
    }

    private boolean clicked = false;

    public void demoWindow() {
        ImGui.begin("Demo Window");
        ImGui.text("Text 1");
        ImGui.selectable("Selectable");
        if (ImGui.button("Click me!")) {
            clicked = !clicked;
        }
        if (clicked) {
            ImGui.sameLine();
            ImGui.text("Clicked!");
        }

        ImGui.bullet();
        ImGui.text("Bullet?");
        ImGui.end();
    }

    public void endFrame() {
        ImGui.render();
        imGuiGl3.renderDrawData(ImGui.getDrawData());

        if (ImGui.getIO().hasConfigFlags(ImGuiConfigFlags.ViewportsEnable)) {
            final long backupWindowPtr = GLFW.glfwGetCurrentContext();
            ImGui.updatePlatformWindows();
            ImGui.renderPlatformWindowsDefault();
            GLFW.glfwMakeContextCurrent(backupWindowPtr);
        }
    }

    public void render(Scene scene) {
        newFrame();
        scene.imgui();
        ImGui.showDemoWindow();
        endFrame();
    }

    public void dispose() {
        imGuiGl3.dispose();
        imGuiGlfw.dispose();
        ImGui.destroyContext();
    }
}
