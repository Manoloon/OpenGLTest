#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 dirLightSpacePos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 dirLightTransform;

void main()              
{                  
    gl_Position = projection * view * model * vec4(pos, 1.0);
    dirLightSpacePos = dirLightTransform * model * vec4(pos,1.0);
    vCol = vec4(clamp(pos,0.0f,1.0f),1.0f);    

    TexCoord = tex;

    // convert mat3 = we dont need the 4th column which is transform in our model.
    // inverse and transpose = if we scaling in 1 dimension we are going to face problems, 
    // so we inverse and then transpose the model. 
    
    Normal = mat3(transpose(inverse(model))) * norm;
    
    FragPos = (model * vec4(pos,1.0f)).xyz;
}