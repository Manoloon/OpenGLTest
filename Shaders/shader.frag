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

struct OmniShadowMap
{
    samplerCube shadowMap;
    float farPlane;
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
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];
uniform Material material;

// this is for PCF (avoid a nested for loop)
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float CalcOmniShadowFactor(vec3 lightPos, int shadowIndex)
{
    vec3 fragToLight = FragPos - lightPos;
    float current = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.05;
    int samples = 20;
    float viewDist = length(eyePosition - FragPos);
    float diskRadius = (1.0 + (viewDist / omniShadowMaps[shadowIndex].farPlane)) / 25.0;
    for(int i = 0; i < samples; i++)
    {
        float closest = texture(omniShadowMaps[shadowIndex].shadowMap, 
                                fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closest *= omniShadowMaps[shadowIndex].farPlane;
        if(current - bias > closest)
        {
            shadow += 1.0;
        }
    }
    shadow /= float(samples);
    return 1.0; // shadow
}

float CalcShadowFactor(vec4 DirectionalLightSpacePos)
{
    vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
    projCoords = (projCoords * 0.5) + 0.5;

    // here we calc forwards and backwards of the closest point in that direction
    // r is the standard letter to get the first value
    //float closestDepth = texture(directionalShadowMap,projCoords.xy).r;
    float currentDepth = projCoords.z;
   // float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    // solve acne 

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(dirLight.direction);
    float bias = max(0.05 * (1.0 - dot(normal,lightDir)), 0.0005);
    float shadow = 1.0;
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
    
    if (projCoords.z > 1.0)
    {
        shadow = 0.0;
    }
    return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction,float shadowFactor)
{
    vec4 ambientCol = vec4(light.colour,1.0f) * light.ambientIntensity;
    
    // A.B = |A||B|cos(angle)
    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)),0.0f);
    
    vec4 diffuseCol = vec4(light.colour * light.diffuseIntensity * diffuseFactor,1.0f); 
    
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

vec4 CalcDirectionalLight(vec4 DirectionalLightSpacePos)
{   
    float shadowFactor = CalcShadowFactor(DirectionalLightSpacePos);
    return CalcLightByDirection(dirLight.base,dirLight.direction,shadowFactor);
}

vec4 CalcPointLight(PointLight pLight, int shadowIndex)
{
    vec3 direction = FragPos - pLight.position;
    float dist = length(direction);
    direction = normalize(direction);

    float shadowFactor = CalcOmniShadowFactor(pLight.position,shadowIndex);

    vec4 color = CalcLightByDirection(pLight.base,direction,shadowFactor);
    // ax^2 + bx + c ; x = distance
    float attenuation = pLight.exponent * dist * dist + 
                        pLight.linear * dist +
                        pLight.constant;
                        
    if(attenuation > 0)
    {
        return (color/attenuation);
    }
    return (color);
}

vec4 CalcTotalPointLights()
{
   vec4 totalColor= vec4(0,0,0,0);
   for(int i = 0; i < pointLightCount; i++)
   {
    totalColor += CalcPointLight(pLights[i], i);
   }
   return totalColor;
}

vec4 CalcSpotLight(SpotLight sLight, int shadowIndex)
{
    vec3 rayDir = normalize(FragPos - sLight.base.position);
    float spotLightFactor = dot(rayDir, sLight.direction);

    if(spotLightFactor > sLight.edge)
    {
        vec4 colour = CalcPointLight(sLight.base,shadowIndex);
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
        totalColor += CalcSpotLight(sLights[i],i + pointLightCount);
    }
    return totalColor;
}

void main()         
{               
    vec4 finalColor = CalcDirectionalLight(DirLightSpacePos);
    
    finalColor += CalcTotalPointLights();
    finalColor += CalcTotalSpotLights();
    colour = texture(theTexture,TexCoord) * finalColor;
}