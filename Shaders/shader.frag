#version 330
              
in vec4 vCol;   
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour; 

struct DirectionalLight{
    vec3 colour;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

struct Material{
    float specularIntensity;
    float shininess;
};

uniform sampler2D theTexture;
uniform DirectionalLight dirLight;
uniform Material material;
uniform vec3 eyePosition;

void main()         
{               
    vec4 ambientCol = vec4(dirLight.colour,1.0f) * dirLight.ambientIntensity;
    
    // A.B = |A||B|cos(angle)
    float diffuseFactor = max(dot(normalize(Normal), normalize(dirLight.direction)),0.0f);
    
    vec4 diffuseCol = vec4(dirLight.colour,1.0f) * dirLight.diffuseIntensity * diffuseFactor; 
    
    vec4 specularCol = vec4(0,0,0,0);
    
    if(diffuseFactor > 0.0f){
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(dirLight.direction,normalize(Normal)));
        float specularFactor = dot(fragToEye,reflectedVertex);
        if(specularFactor > 0.0f){
            specularFactor = pow(specularFactor,material.shininess);
            specularCol = vec4(dirLight.colour * material.specularIntensity * specularFactor,1.0f);
        }
    }
    colour = texture(theTexture,TexCoord) * (ambientCol + diffuseCol + specularCol);
}