#version 330
              
in vec4 vCol;   
in vec2 TexCoord;
in vec3 Normal;

out vec4 colour; 

struct DirectionalLight{
    vec3 colour;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

uniform sampler2D theTexture;
uniform DirectionalLight dirLight;

void main()         
{               
    vec4 ambientCol = vec4(dirLight.colour,1.0f) * dirLight.ambientIntensity;
    
    // A.B = |A||B|cos(angle)
    float diffuseFactor = max(dot(normalize(Normal), normalize(dirLight.direction)),0.0f);
    
    vec4 diffuseColor = vec4(dirLight.colour,1.0f) * dirLight.diffuseIntensity * diffuseFactor; 
    
    colour = texture(theTexture,TexCoord) * (ambientCol + diffuseColor);
}