# OpenGL Rendering pipeline
- 4 stages : shaders
- GLSL (OpenGL Shading language) -> c based
- HLSL (High-level shading language) -> direct3d

# Pipeline stages
1 - vertex Spec
	- (x,y,z) point in space
	- Uses VAOs (Vertex Array Objs) and VBOs (Vertex Buffer Objs)
	- VAO = define what data a vertex has (position,colour,texture,normals,etc)
	- BVO = define the data itself Vertex->GetPosition()
	- Attribute pointers = define where and how shaders can access vertex data (connectors)
	- After create the VAO and BVO , define the attrb pointer and enable it
	- then Init draw as -> activate shader program, bind VAO you want to draw and Call glDrawArrays, which init the rest of the pipeline.

2 - vertex shader
	- Handle vertices individially.
	- NOT Optional
	- Must store something in gl_Position for later stages.

3 - tessellation
	- Allows to divide data into smaller primitives

4 - Geometry shader (fancy explosions)
	- takes primitives then emits their vertices to create the given or new primitives.

5 - Vertex post-process (clipping) dont draw what cant see
	- Result of vertex and geometry stages saved to buffer .

6 - Primitive Assembly
	- vertices are converted in to a series of primitives (6 vertices become 2 triangles)
	- Face culling : remove primitives that cant be seen or facing away from viewer. 

7 - Rasterization
	- Convert primitives into fragments. Interpolated based on its position related to each vertex.

8 - Fragment shader (not used when shadow maps)
	- Handles data for each fragment.
	- The most common shader, most important output is the color of the pixel that the fragment covers.

9 - Per-sample ops.
	- Series of tests run to see if the fragment should be drawn.
	- Most important test : Depth Tests. What is in front of what.
	- Color Blending by overlapping fragments. For Handle transparency.
