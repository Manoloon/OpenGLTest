# OpenGL Test project
## Projections
- Local Space : Raw position of vertex. Multiply by Model Matrix gets =
- World Space : Position of vertex in the world. Multiply by View Matrix gets =
- View Space : Position of vertex in the world relative to the camera position and rotation. Multiply by Projection Matrix gets =
- Clip Space : Position of vertex in the world relative to the camera position and rotation as viewed in the area "Not to be "Clipped "". 
- Screen Space : Actual Clipping taking place.
![](https://github.com/Manoloon/OpenGLTest/blob/main/opengl.gif)
