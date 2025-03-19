#version 150

// Matrices for transforming position
uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

// Vertex attributes
in vec4 a_position;
in vec2 a_uv1;
in vec3 a_normal; // ADD NORMALS

// Pass to fragment shader
out vec2 v_uv1;
out vec3 v_normal;
out vec3 v_fragPosition;

void main()
{
    vec4 worldPosition = world * a_position;
    gl_Position = projection * view * worldPosition;

    v_uv1 = a_uv1;
    v_normal = mat3(transpose(inverse(world))) * a_normal; // Transform normals correctly
    v_fragPosition = vec3(worldPosition);
}


/*
#version 150
uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
in vec4 a_position;
in vec2 a_uv1;
out vec2 v_uv1;

void main()
{
    gl_Position = projection * view * world * a_position;
	v_uv1 = a_uv1;
}
*/