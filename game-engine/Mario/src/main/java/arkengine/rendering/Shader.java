package arkengine.rendering;

import org.joml.*;
import org.lwjgl.BufferUtils;

import java.io.IOException;
import java.nio.FloatBuffer;
import java.nio.file.Files;
import java.nio.file.Paths;

import static org.lwjgl.opengl.GL20.*;

public class Shader {
    private String shaderPath;
    private String vertexSource; // Currently running vertex source
    private String fragmentSource; // Currently running fragment source

    private int shaderProgramID;
    private int vertexID, fragmentID;

    public Shader(String filepath) {
        this.shaderPath = filepath;
        parseFile();
    }

    private void parseFile() {
        try {
            String source = new String(Files.readAllBytes(Paths.get(shaderPath)));
            String[] splitString = source.split("(?=(#type)( )+([a-zA-Z]+))");

            // Find the first shader type
            for (String s : splitString) {
                if (s.trim().isEmpty()) continue;

                int shaderTypeStart = 6; // Skip the first 6 characters: "#type "
                int eol = s.indexOf('\n', shaderTypeStart); // End of the shader type string
                String shaderType = s.substring(shaderTypeStart, eol).trim().toLowerCase();

                String shaderSource = s.substring(eol).trim(); // trim off the type header

                if (shaderType.equals("vertex")) {
                    vertexSource = shaderSource;
                } else if (shaderType.equals("fragment")) {
                    fragmentSource = shaderSource;
                } else {
                    // ignore it
                    System.out.println("Unsupported shader type: " + shaderType);
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
            throw new RuntimeException("Could not open file for shader: " + shaderPath);
        }
    }

    public boolean compile() {
        // --- Compile vertexSource
        vertexID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexID, vertexSource);
        glCompileShader(vertexID);

        int success = glGetShaderi(vertexID, GL_COMPILE_STATUS);
        if (success == GL_FALSE) {
            int len = glGetShaderi(vertexID, GL_INFO_LOG_LENGTH);
            System.out.println("ERROR: " + this.shaderPath + " : Vertex shader compilation failed.");
            System.out.println(glGetShaderInfoLog(vertexID, len));
            return false;
        }

        // --- Compile fragmentSource
        fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentID, fragmentSource);
        glCompileShader(fragmentID);

        success = glGetShaderi(fragmentID, GL_COMPILE_STATUS);
        if (success == GL_FALSE) {
            int len = glGetShaderi(fragmentID, GL_INFO_LOG_LENGTH);
            System.out.println("ERROR: " + this.shaderPath + " : Fragment shader compilation failed.");
            System.out.println(glGetShaderInfoLog(fragmentID, len));
            return false;
        }

        // --- Link
        shaderProgramID = glCreateProgram();
        glAttachShader(shaderProgramID, vertexID);
        glAttachShader(shaderProgramID, fragmentID);
        glLinkProgram(shaderProgramID);

        success = glGetProgrami(shaderProgramID, GL_LINK_STATUS);
        if (success == GL_FALSE) {
            int len = glGetProgrami(shaderProgramID, GL_INFO_LOG_LENGTH);
            System.out.println("ERROR: " + this.shaderPath + " : Program linking failed.");
            System.out.println(glGetProgramInfoLog(shaderProgramID, len));
            return false;
        }

        return true;
    }

    public boolean reload() {
        parseFile();
        return compile();
    }

    public void bind() {
        glUseProgram(shaderProgramID);
    }

    public void detach() {
        glUseProgram(0);
    }

    public void setUniformMat4f(String varName, Matrix4f mat) {
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        bind();
        FloatBuffer matBuffer = BufferUtils.createFloatBuffer(16);
        mat.get(matBuffer);
        glUniformMatrix4fv(varLocation, false, matBuffer);
    }

    public void setUniformMat3f(String varName, Matrix3f mat) {
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        bind();
        FloatBuffer matBuffer = BufferUtils.createFloatBuffer(9);
        mat.get(matBuffer);
        glUniformMatrix3fv(varLocation, false, matBuffer);
    }

    public void setUniformMat2f(String varName, Matrix2f mat) {
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        bind();
        FloatBuffer matBuffer = BufferUtils.createFloatBuffer(4);
        mat.get(matBuffer);
        glUniformMatrix2fv(varLocation, false, matBuffer);
    }

    public void setUniformVec4f(String varName, Vector4f vec) {
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        bind();
        glUniform4f(varLocation, vec.x, vec.y, vec.z, vec.w);
    }

    public void setUniformVec3f(String varName, Vector3f vec) {
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        bind();
        glUniform3f(varLocation, vec.x, vec.y, vec.z);
    }

    public void setUniformVec2f(String varName, Vector2f vec) {
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        bind();
        glUniform2f(varLocation, vec.x, vec.y);
    }

    public void setUniform1f(String varName, float val) {
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        bind();
        glUniform1f(varLocation, val);
    }

    public void setUniform1i(String varName, int val) {
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        bind();
        glUniform1i(varLocation, val);
    }
}
