
# OpenGL Test project

## Projections

- Local Space : Raw position of vertex. Multiply by Model Matrix gets =
- World Space : Position of vertex in the world. Multiply by View Matrix gets =
- View Space : Position of vertex in the world relative to the camera position and rotation. Multiply by Projection Matrix gets =
- Clip Space : Position of vertex in the world relative to the camera position and rotation as viewed in the area "Not to be "Clipped "".
- Screen Space : Actual Clipping taking place.

## Camera and Input

## texture Sampler

- using stb_image as image loader.

## Lighting

# Phong Lighting model
-ambient : always present
-diffuse : determine by direction of light
	- diffuse factor = dotproduct v1 and v2 . cos(0 degrees) = 1 , cos (90 degrees) = 0
cos(45 degrees) = 0.71.

-specular : reflected perfectly from source to viewer.
-Combine Ambient, Diffuse and Specular.

Flat Shading : use each normal from the faces.

Phong shading is good for complex models not so for simple ones.
each vertex have an average of the normals of all the surfaces it is part of.

# Lights : Directional Light , Point Light and Spot Light

# Using Assimp to import models

# Shadow Mapping
# Directional shadow
![directional shadow](https://learnopengl.com/img/advanced-lighting/shadow_mapping_theory.png)

![directional shadow](https://learnopengl.com/img/advanced-lighting/shadow_mapping_theory_spaces.png)

It create a texture2D by calculating where the rays of lights are hitting and where they not. 
It use a framebuffer as holder and then this information is store in the texture.
At least 2 rendering passes are needed : 1 for create the shadow map and a second one to draw the scene.
-Steps:
	The first step is taking from the light point of view.
	GLGenFramebuffers(1,&FBO) will create the framebuffer.
	GLTexImage2D : is used but , instead of a GL_RGBA with use a G:_DEPTH_COMPONENT which have one float holding Depth.
	And as DATA need to pass nullptr, in this way the texture is created empty. This will be update with the framebuffer.
	* GLFramebufferTexture2D(GL_Framebuffer,GL_DEPTH_COMPONENT,GL_TEXTURE2D,shadowmap,0);
For the shadow map we dont need colours, its only a range from 0 to 1 on white.
After render the scene with the shadow map shader, the texture bounded is fill with shadow map data.
Then is mandatory to unbound this texture from the framebuffer, and bound it to the main shader to use it.

The texture is black and white and orthographic , to this we need to divide the perspective :
In the shader we need to handle this:
* vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w (lightspacePos is a vec4)
Perspective Divide :
ProjCoords = (ProjCoords * 0.5) + 0.5 // this formula scale the coords from 0 to 1 .
We need it this way because UVs are in range 0-1.

To get the closest depth measure during shadow map pass : 
// r is the standard letter to get the first value
float closest = texture(shadowMap,ProjCoords.xy).r
float current = normalize(ProjCoords.z);
if(current > closest) -> is a shadow 
else -> is hitting by light.
- To Apply shadow map 
* colour = fragCol * (ambient + (1.0 - shadow) * (diffuse + specular));
Remember Ambient light is always there.
(1.0 - shadow ) = will scatter shadows.

Terminology : 
Shadow Acne :
Peter Panning : Shadow shift away from the object due to the bias applied being to large.
PCF : Percentage-closer filtering .


# omni shadow maps

Cubemap !
cubemap counts 6 textures but can be ref as 1 texture in GLSL
bind = GL_TEXTURE_CUBE_MAP
teximage2D = GL_TEXTURE_CUBE_MAP_POSITIVE_X, NEGATIVE_X, POSITIVE_Y, NEGATIVE_Y
POSITIVE_Z, NEGATIVE_Z ( 6 in total)

![pointlights shadow](https://learnopengl.com/img/advanced-lighting/point_shadows_diagram.png)

dont use UVs 
can access point on cube map with direction vector pointing to Texel on cube map from center of the cube.

use perspective projection.
aspect = width of one side of cube / height. Should be always 1 .

Geometry Shader
Prjection and view will be applied in the Geo Shader.
happens btw vertex and fragment shaders.
handle primitives (points,lines,triangles)
Can create new primitives
samplerCube is where Cubemap should be bind with.
PCF is the same but with a vec3 now, hence more intensive.
![Alt Text](https://github.com/Manoloon/OpenGLTest/blob/main/opengl.gif)
