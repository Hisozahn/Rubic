#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 Color;
out vec2 TexCoord;
out vec3 Pos;

void main()
{
    gl_Position = projection * view * model * vec4(Position, 1.0);
    Pos = Position;
    Color = vec4(clamp(Position, 0.0, 1.0), 1.0);
    TexCoord = aTexCoord;
}