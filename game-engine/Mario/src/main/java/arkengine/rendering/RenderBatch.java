package arkengine.rendering;

import arkengine.Window;
import arkengine.components.SpriteRenderer;
import arkengine.components.Transform;
import org.joml.Vector4f;

import static org.lwjgl.opengl.GL15.*;
import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.opengl.GL20.glDisableVertexAttribArray;
import static org.lwjgl.opengl.GL30.glBindVertexArray;
import static org.lwjgl.opengl.GL30.glGenVertexArrays;

public class RenderBatch {
    // Vertex layout
    // ===================
    // position         color
    // float, float,    float, float, float, float
    // ===================
    private final int POS_SIZE = 2;
    private final int COL_SIZE = 4;

    private final int POS_OFFSET = 0;
    private final int COL_OFFSET = POS_OFFSET + POS_SIZE * Float.BYTES;

    private final int VERTEX_SIZE = 6;
    private final int VERTEX_SIZE_BYTES = VERTEX_SIZE * Float.BYTES;

    private int numSprites = 0;
    private boolean hasRoom = true;

    private Shader shader;
    private SpriteRenderer[] sprites;
    private float[] vertices;
    private int vaoID, vboID;
    private int maxBatchSize;

    public RenderBatch(int maxBatchSize) {
        this.maxBatchSize = maxBatchSize;
        shader = new Shader("assets/shaders/default.glsl");
        shader.compile();

        sprites = new SpriteRenderer[maxBatchSize];

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
    }

    public void addSprite(SpriteRenderer sr) {
        if (!hasRoom) return;

        sprites[numSprites++] = sr;
        loadVertexProperties(numSprites - 1);

        hasRoom = numSprites < maxBatchSize;
    }

    private void loadVertexProperties(int index) {
        SpriteRenderer sprite = sprites[index];

        int offset = index * 4 * VERTEX_SIZE;
        Vector4f color = sprite.getColor();

        // add vertices with appropriate properties
        float xAdd = 1.0f;
        float yAdd = 1.0f;
        for (int i = 0; i < 4; i++) {
            switch (i) {
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
            Transform trf =  sprite.gameObject.getComponent(Transform.class);
            vertices[offset] = trf.position.x + (xAdd * trf.scale.x);
            vertices[offset + 1] = trf.position.y + (yAdd * trf.scale.y);
            vertices[offset + 2] = color.x;
            vertices[offset + 3] = color.y;
            vertices[offset + 4] = color.z;
            vertices[offset + 5] = color.w;

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
        // For now, rebuffer all data every frame
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices);

        // Use the shader
        shader.bind();
        shader.setUniformMat4f("uProj", Window.getScene().getCamera().getProjMatrix());
        shader.setUniformMat4f("uView", Window.getScene().getCamera().getViewMatrix());

        glBindVertexArray(vaoID);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glDrawElements(GL_TRIANGLES, numSprites * 6, GL_UNSIGNED_INT, 0);

        // Unbind everything
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindVertexArray(0);
        shader.detach();
    }

    public boolean hasRoom() {
        return hasRoom;
    }
}
