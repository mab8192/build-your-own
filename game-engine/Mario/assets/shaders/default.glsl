#type vertex
#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec4 aCol;
layout(location=2) in vec2 aTexCoords;
layout(location=3) in float aTexID;

uniform mat4 uProj;
uniform mat4 uView;

out vec4 fColor;
out vec2 fTexCoords;
out float fTexID;

void main()
{
    fColor = aCol;
    fTexCoords = aTexCoords;
    fTexID = aTexID;
    gl_Position = uProj * uView * vec4(aPos, 1.0);
}

#type fragment
#version 330 core

in vec4 fColor;
in vec2 fTexCoords;
in float fTexID;

uniform sampler2D uTextures[8];

out vec4 color;

void main()
{
    if (fTexID > 0) {
        int id = int(fTexID);
        color = fColor * texture(uTextures[id], fTexCoords);
    } else {
        color = fColor;
    }

}
