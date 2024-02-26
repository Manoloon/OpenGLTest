#version 330
              
in vec4 vCol;   
in vec2 TexCoord;

out vec4 colour; 

struct DirectionalLight{
    vec3 colour;
    float ambientIntensity;
};

uniform sampler2D theTexture;
uniform DirectionalLight dirLight;

void main()         
{               
    vec4 ambientCol = vec4(dirLight.colour,1.0f) * dirLight.ambientIntensity;
    colour = texture(theTexture,TexCoord) * ambientCol;
}