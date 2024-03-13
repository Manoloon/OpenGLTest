#version 330
              
in vec4 vCol;   
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirLightSpacePos;

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

uniform DirectionalLight dirLight;
uniform PointLight pLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;
uniform SpotLight sLights[MAX_SPOT_LIGHTS];
uniform int spotLightCount;
uniform vec3 eyePosition;

// Texture0
uniform sampler2D theTexture;
// Texture1
uniform sampler2D directionalShadowMap;
uniform Material material;

float CalcShadowFactor(DirectionalLight light)
{
    vec3 projCoords = DirLightSpacePos.xyz / DirLightSpacePos.w;
    projCoords = (projCoords * 0.5) + 0.5;

    // here we calc forwards and backwards of the closest point in that direction
    // r is the standard letter to get the first value
    //float closestDepth = texture(directionalShadowMap,projCoords.xy).r;
    float currentDepth = projCoords.z;
   // float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    // solve acne 

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.direction);
    float bias = max(0.05 * (1.0 - dot(normal,lightDir)), 0.0005);
    float shadow = 0.0;
    // PCF
    vec2 texelSize = 1.0 / textureSize(directionalShadowMap,0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(directionalShadowMap,projCoords.xy + vec2(x,y) * texelSize).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    ////////
    if (projCoords.z > 1.0)
    {
        shadow = 0.0;
    }
    ///
    return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction,float shadowFactor)
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
    return (ambientCol + (1.0f - shadowFactor) * (diffuseCol + specularCol));
}

vec4 CalcDirectionalLight()
{   
    float shadowFactor = CalcShadowFactor(dirLight);
    return CalcLightByDirection(dirLight.base,dirLight.direction,shadowFactor);
}

vec4 CalcPointLight(PointLight pLight)
{
    vec3 direction = FragPos - pLight.position;
    float distance = length(direction);
    direction = normalize(direction);
    float shadow = 0.0f;

    vec4 color = CalcLightByDirection(pLight.base,direction,shadow);
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