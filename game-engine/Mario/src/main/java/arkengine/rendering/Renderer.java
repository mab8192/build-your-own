package arkengine.rendering;

import arkengine.components.SpriteRenderer;
import arkengine.scene.GameObject;
import arkengine.scene.Scene;
import imgui.ImFontAtlas;
import imgui.ImFontConfig;
import imgui.ImGui;
import imgui.ImGuiIO;
import imgui.flag.ImGuiConfigFlags;
import imgui.flag.ImGuiFreeTypeBuilderFlags;
import imgui.gl3.ImGuiImplGl3;
import imgui.glfw.ImGuiImplGlfw;
import org.lwjgl.glfw.GLFW;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class Renderer {
    private final int MAX_BATCH_SIZE = 1000;
    private final List<RenderBatch> renderBatches = new ArrayList<>();

    private final ImGuiImplGlfw imGuiGlfw = new ImGuiImplGlfw();
    private final ImGuiImplGl3 imGuiGl3 = new ImGuiImplGl3();

    public Renderer(long glfwWindow) {
        // Init ImGui
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

    /**
     * Submit a game object to be included in a render pass
     * @param obj The object to be drawn. Should have a SpriteRenderer component.
     */
    public void submit(GameObject obj) {
        ArrayList<SpriteRenderer> sprs = obj.getAllComponents(SpriteRenderer.class);
        for (SpriteRenderer spr : sprs) {
            add(spr);
        }
    }

    private void add(SpriteRenderer spr) {
        boolean added = false;
        for (RenderBatch batch : renderBatches) {
            if (batch.hasRoom() && batch.getzIndex() == spr.gameObject.getzIndex()) {
                Texture tex = spr.getTexture();
                if (tex == null || (batch.hasTexture(tex) || batch.hasTextureRoom())) {
                    batch.addSprite(spr);
                    added = true;
                    break;
                }
            }
        }

        if (!added) {
            RenderBatch batch = new RenderBatch(MAX_BATCH_SIZE, spr.gameObject.getzIndex());
            batch.start();
            batch.addSprite(spr);

            renderBatches.add(batch);

            // Ensure z index of batches is in the correct order
            renderBatches.sort(Comparator.comparingInt(RenderBatch::getzIndex));
        }
    }

    private void newImGuiFrame() {
        imGuiGlfw.newFrame();
        ImGui.newFrame();
    }

    private void endImGuiFrame() {
        ImGui.render();
        imGuiGl3.renderDrawData(ImGui.getDrawData());

        if (ImGui.getIO().hasConfigFlags(ImGuiConfigFlags.ViewportsEnable)) {
            final long backupWindowPtr = GLFW.glfwGetCurrentContext();
            ImGui.updatePlatformWindows();
            ImGui.renderPlatformWindowsDefault();
            GLFW.glfwMakeContextCurrent(backupWindowPtr);
        }
    }

    /**
     * Render previously submitted draw calls.
     * @param scene The active scene, passed along for ImGui
     */
    public void render(Scene scene) {
        // Draw the submitted objects to the screen
        for (RenderBatch batch : renderBatches) {
            batch.render();
        }

        newImGuiFrame();
        scene.imgui();
        ImGui.showDemoWindow();
        endImGuiFrame();
    }

    public void dispose() {
        imGuiGl3.dispose();
        imGuiGlfw.dispose();
        ImGui.destroyContext();
    }
}
