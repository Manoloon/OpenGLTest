#version 330
// this data came from the vertex shader.
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 lightMatrices[6];

// this is the data that will be passes to the fragment shader.
out vec4 FragPos;

void main()
{
    for(int face = 0; face < 6 ; face++)
    {
        gl_Layer = face;
        // 3 represent each point in a triangle
        for(int i = 0; i < 3; i++)
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = lightMatrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}