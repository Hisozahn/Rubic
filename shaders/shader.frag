#version 330

out vec4 FragColor;

in vec4 Color;
in vec2 TexCoord;
in vec3 Pos;

uniform sampler2D textureFront;
uniform sampler2D textureRight;
uniform sampler2D textureBack;
uniform sampler2D textureLeft;
uniform sampler2D textureTop;
uniform sampler2D textureBottom;

const float epsilon = 0.00001;

void main()
{
    if (abs(Pos.x - 1.0) < epsilon) {
        FragColor = texture(textureRight, TexCoord);
    }
    else if (abs(Pos.x + 1.0) < epsilon) {
        FragColor = texture(textureLeft, TexCoord);
    }
    else if (abs(Pos.z - 1.0) < epsilon) {
        FragColor = texture(textureFront, TexCoord);
    }
    else if (abs(Pos.z + 1.0) < epsilon) {
        FragColor = texture(textureBack, TexCoord);
    }
    else if (abs(Pos.y - 1.0) < epsilon) {
        FragColor = texture(textureTop, TexCoord);
    }
    else if (abs(Pos.y + 1.0) < epsilon) {
        FragColor = texture(textureBottom, TexCoord);
    }
}