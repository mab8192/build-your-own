package emergentworlds.rendering;

import org.joml.Vector2f;
import org.joml.Vector4f;

public interface IRenderable {
    public Vector2f getWorldPosition();
    public Vector2f getOffset();
    public Vector4f getColor();
    public Vector2f getScale();
    public Texture getTexture();
    public Vector2f[] getTexCoords();

    public boolean isDirty();
    public void setClean();
    public void setDirty();
}
