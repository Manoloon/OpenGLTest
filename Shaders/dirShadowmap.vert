#version 330

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 dirLightTransform;

void main()
{
    gl_Position = dirLightTransform * model * vec4(pos,1.0);
}