package emergentworlds.rendering;

import emergentworlds.Window;
import emergentworlds.components.SpriteRenderer;
import emergentworlds.components.Transform;
import emergentworlds.util.AssetPool;
import org.joml.Vector2f;
import org.joml.Vector4f;

import java.util.ArrayList;
import java.util.List;

import static org.lwjgl.opengl.GL15.*;
import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.opengl.GL20.glDisableVertexAttribArray;
import static org.lwjgl.opengl.GL30.glBindVertexArray;
import static org.lwjgl.opengl.GL30.glGenVertexArrays;

public class RenderBatch {
    // Vertex layout
    // ===================
    // position         color                           tex coords      tex id
    // float, float,    float, float, float, float,     float, float,   float
    // ===================
    private final int POS_SIZE = 2;
    private final int COL_SIZE = 4;
    private final int TEX_COORDS_SIZE = 2;
    private final int TEX_ID_SIZE = 1;

    private final int POS_OFFSET = 0;
    private final int COL_OFFSET = POS_OFFSET + POS_SIZE * Float.BYTES;
    private final int TEX_COORDS_OFFSET = COL_OFFSET + COL_SIZE * Float.BYTES;
    private final int TEX_ID_OFFSET = TEX_COORDS_OFFSET + TEX_COORDS_SIZE * Float.BYTES;

    private final int VERTEX_SIZE = 9;
    private final int VERTEX_SIZE_BYTES = VERTEX_SIZE * Float.BYTES;

    private int numSprites = 0;
    private boolean hasRoom = true;

    private Shader shader;
    private SpriteRenderer[] spriteRenderers;
    private List<Texture> textures = new ArrayList<>();
    private float[] vertices;
    private int[] texSlots = {0, 1, 2, 3, 4, 5, 6, 7};
    private int vaoID, vboID;
    private int maxBatchSize;
    private int zIndex;

    public RenderBatch(int maxBatchSize, int zIndex) {
        this.zIndex = zIndex;
        this.maxBatchSize = maxBatchSize;
        shader = AssetPool.getShader("assets/shaders/default.glsl");

        spriteRenderers = new SpriteRenderer[maxBatchSize];

        // 6 floats per vertex, 4 vertices per quad
        vertices = new float[maxBatchSize * 4 * VERTEX_SIZE];
    }

    public void start() {
        // Create data on the GPU
        // Generate and bind VAO
        vaoID = glGenVertexArrays();
        glBindVertexArray(vaoID);

        // Allocate space for vertices
        vboID = glGenBuffers();
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, vertices.length * Float.BYTES, GL_DYNAMIC_DRAW); // dynamic draw instead of static

        // Create indices
        int eboID = glGenBuffers();
        int[] indices = generateIndices();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);

        // Enable buffer attribute pointers
        glVertexAttribPointer(0, POS_SIZE, GL_FLOAT, false, VERTEX_SIZE_BYTES, POS_OFFSET);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, COL_SIZE, GL_FLOAT, false, VERTEX_SIZE_BYTES, COL_OFFSET);
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, TEX_COORDS_SIZE, GL_FLOAT, false, VERTEX_SIZE_BYTES, TEX_COORDS_OFFSET);
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, TEX_ID_SIZE, GL_FLOAT, false, VERTEX_SIZE_BYTES, TEX_ID_OFFSET);
        glEnableVertexAttribArray(3);
    }

    public void addSprite(SpriteRenderer sr) {
        if (!hasRoom) return;

        spriteRenderers[numSprites++] = sr;
        if (sr.getTexture() != null) {
            if (!textures.contains(sr.getTexture())) {
                textures.add(sr.getTexture());
            }
        }
        loadVertexProperties(numSprites - 1);

        hasRoom = numSprites < maxBatchSize;
    }

    private void loadVertexProperties(int index) {
        SpriteRenderer spr = spriteRenderers[index];

        int offset = index * 4 * VERTEX_SIZE;
        Vector4f color = spr.getColor();
        Vector2f[] texCoords = spr.getTexCoords();

        int texID = 0;
        if (spr.getTexture() != null)
            texID = textures.indexOf(spr.getTexture()) + 1;

        // add vertices with appropriate properties
        float xAdd = 1.0f;
        float yAdd = 1.0f;
        for (int vertexNum = 0; vertexNum < 4; vertexNum++) {
            switch (vertexNum) {
                case 1:
                    yAdd = 0.0f;
                    break;
                case 2:
                    xAdd = 0.0f;
                    break;
                case 3:
                    yAdd = 1.0f;
                    break;
                default:
                    break;
            }

            // Load position
            Transform trf =  spr._entity.transform;
            vertices[offset] = trf.position.x + spr.getOffset().x + (xAdd * trf.scale.x);
            vertices[offset + 1] = trf.position.y + spr.getOffset().y + (yAdd * trf.scale.y);

            // Load color
            vertices[offset + 2] = color.x;
            vertices[offset + 3] = color.y;
            vertices[offset + 4] = color.z;
            vertices[offset + 5] = color.w;

            // Load texture coords
            vertices[offset + 6] = texCoords[vertexNum].x;
            vertices[offset + 7] = texCoords[vertexNum].y;

            // Load texture id
            vertices[offset + 8] = texID;

            offset += VERTEX_SIZE;
        }
    }

    private int[] generateIndices() {
        // 6 indices per quad, or 3 per triangle
        int[] elements = new int[6 * maxBatchSize];
        for (int i = 0; i < maxBatchSize; i++) {
            loadElementIndices(elements, i);
        }
        return elements;
    }

    private void loadElementIndices(int[] elements, int index) {
        int offsetArrayIndex = 6*index;
        int offset = 4*index;

        // 3,2,0,0,2,1   7,6,4,4,6,5   ...
        // Triangle 1 (3, 2, 0)
        elements[offsetArrayIndex] = offset + 3;
        elements[offsetArrayIndex+1] = offset + 2;
        elements[offsetArrayIndex+2] = offset + 0;

        // Triangle 2 (0, 2, 1)
        elements[offsetArrayIndex + 3] = offset + 0;
        elements[offsetArrayIndex + 4] = offset + 2;
        elements[offsetArrayIndex + 5] = offset + 1;
    }

    public void render() {
        boolean rebufferNeeded = false;

        for (int i = 0; i < numSprites; i++) {
            SpriteRenderer spr = spriteRenderers[i];
            if (spr.isDirty()) {
                loadVertexProperties(i);
                spr.setClean();
                rebufferNeeded = true;
            }
        }

        // Skip rebuffer if no sprites need to be redrawn
        if (rebufferNeeded) {
            glBindBuffer(GL_ARRAY_BUFFER, vboID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertices);
        }

        // Use the shader
        shader.bind();
        shader.setUniformMat4f("uProj", Window.getWorld().getCamera().getProjMatrix());
        shader.setUniformMat4f("uView", Window.getWorld().getCamera().getViewMatrix());

        // Bind textures
        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE1 + i); // use texture 0 as the "no texture, color only" texture slot
            textures.get(i).bind();
        }
        shader.setUniform1iv("uTextures", texSlots);

        glBindVertexArray(vaoID);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glDrawElements(GL_TRIANGLES, numSprites * 6, GL_UNSIGNED_INT, 0);

        // Unbind everything
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glBindVertexArray(0);

        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            textures.get(i).unbind();
        }

        shader.detach();
    }

    public boolean hasRoom() {
        return hasRoom;
    }
    public boolean hasTextureRoom() { return textures.size() < 8; }
    public boolean hasTexture(Texture tex) { return textures.contains(tex); }
    public int getzIndex() { return zIndex; }
}
