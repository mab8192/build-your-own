package emergentworlds.rendering;

import org.joml.Matrix4f;
import org.joml.Vector3f;

public class Camera {
    private final Matrix4f projMatrix, viewMatrix;
    private Vector3f clearColor = new Vector3f(0, 0, 0);
    public Vector3f position;

    public Camera() {
        this(new Vector3f(0f, 0f, 20f));
    }

    public Camera(Vector3f position) {
        this.position = position;
        this.projMatrix = new Matrix4f();
        this.viewMatrix = new Matrix4f();
        adjustProjection();
    }

    public static class OrthoSettings {
        public float left, right, bottom, top, zNear, zFar;

        public static OrthoSettings defaultSettings() {
            OrthoSettings settings = new OrthoSettings();
            settings.left = 0f;
            settings.right = 32f * 40f;
            settings.bottom = 0f;
            settings.top = 32f * 21f;
            settings.zNear = 0f;
            settings.zFar = 100f;

            return settings;
        }
    }

    public void adjustProjection() {
        adjustProjection(OrthoSettings.defaultSettings());
    }

    public void adjustProjection(OrthoSettings settings) {
        projMatrix.identity();
        projMatrix.ortho(settings.left, settings.right, settings.bottom, settings.top, settings.zNear, settings.zFar);
    }

    public Matrix4f getViewMatrix() {
        Vector3f cameraFront = new Vector3f(0f, 0f, -1f);
        Vector3f cameraUp = new Vector3f(0f, 1f, 0f);
        viewMatrix.identity();
        viewMatrix.lookAt(
                position,
                cameraFront.add(position.x, position.y, 0f),
                cameraUp);

        return viewMatrix;
    }

    public Matrix4f getProjMatrix () {
        return projMatrix;
    }

    public void setClearColor(Vector3f clearColor) { this.clearColor = clearColor; }
    public Vector3f getClearColor() { return clearColor; }
}
