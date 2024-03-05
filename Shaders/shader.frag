#version 330
              
in vec4 vCol;   
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour; 
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
    vec3 colour;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;
};

struct PointLight
{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float edge;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform sampler2D theTexture;
uniform Material material;

uniform DirectionalLight dirLight;
uniform PointLight pLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;
uniform SpotLight sLights[MAX_SPOT_LIGHTS];
uniform int spotLightCount;
uniform vec3 eyePosition;

vec4 CalcLightByDirection(Light light, vec3 direction)
{
    vec4 ambientCol = vec4(light.colour,1.0f) * light.ambientIntensity;
    
    // A.B = |A||B|cos(angle)
    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)),0.0f);
    
    vec4 diffuseCol = vec4(light.colour,1.0f) * light.diffuseIntensity * diffuseFactor; 
    
    vec4 specularCol = vec4(0,0,0,0);
    
    if(diffuseFactor > 0.0f)
    {
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(direction,normalize(Normal)));
        float specularFactor = dot(fragToEye,reflectedVertex);
        if(specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor,material.shininess);
            specularCol = vec4(light.colour * material.specularIntensity * specularFactor,1.0f);
        }
    }
    return (ambientCol + diffuseCol + specularCol);
}

vec4 CalcDirectionalLight()
{
    return CalcLightByDirection(dirLight.base,dirLight.direction);
}

vec4 CalcPointLight(PointLight pLight)
{
    vec3 direction = FragPos - pLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    vec4 color = CalcLightByDirection(pLight.base,direction);
    // ax^2 + bx + c ; x = distance
    float attenuation = pLight.exponent * distance * distance + 
                        pLight.linear * distance +
                        pLight.constant;
    return (color/attenuation);
}

vec4 CalcTotalPointLights()
{
   vec4 totalColor= vec4(0,0,0,0);
   for(int i = 0; i < pointLightCount; i++)
   {
    totalColor += CalcPointLight(pLights[i]);
   }
   return totalColor;
}

vec4 CalcSpotLight(SpotLight sLight)
{
    vec3 rayDir = normalize(FragPos - sLight.base.position);
    float spotLightFactor = dot(rayDir, sLight.direction);

    if(spotLightFactor > sLight.edge)
    {
        vec4 colour = CalcPointLight(sLight.base);
        // the ratio between the two scales of the cone = max -(max - spotLightFactor) * (max/(max - sLight.edge))
        return colour * (1.0f - (1.0f - spotLightFactor) * (1.0f/(1.0f - sLight.edge)));
    }
    return vec4(0,0,0,0);
}

vec4 CalcTotalSpotLights()
{
    vec4 totalColor= vec4(0,0,0,0);
    for(int i = 0; i < spotLightCount; i++)
    {
        totalColor += CalcSpotLight(sLights[i]);
    }
    return totalColor;
}

void main()         
{               
    vec4 finalColor = CalcDirectionalLight();
    
    finalColor += CalcTotalPointLights();
    finalColor += CalcTotalSpotLights();
    colour = texture(theTexture,TexCoord) * finalColor;
}