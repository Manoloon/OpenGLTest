#version 330

in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
    float dist = length(FragPos.xyz - lightPos);
    // this normalize it.
    dist = dist/farPlane;
    gl_FragDepth = dist;
}