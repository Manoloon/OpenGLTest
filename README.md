
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
#direction light , point light and spot light.

![Alt Text](https://github.com/Manoloon/OpenGLTest/blob/main/opengl.gif)
