
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
    v_normal = normalize(mat3(world) * a_normal);
    v_fragPosition = vec3(worldPosition);
}

/*
#version 150

// Scene info
uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
//uniform mat3 worldIT;

// Attributes
in vec4 a_position;
in vec2 a_uv1;
in vec3 a_normal;

// Varyings
out vec2 v_uv1;
out vec3 v_normal;
out vec4 v_fragPosition;

void main()
{
    vec4 worldPosition = world * a_position;
    gl_Position = projection * view * worldPosition;
    
    v_uv1 = a_uv1;
    v_normal = mat3(transpose(inverse(world))) * a_normal; //n;
    v_fragPosition = vec3(worldPosition);
}
*/


    //gl_Position = WorldViewProj * a_position;
	//vec3 n = WorldIT * a_normal;
    //v_fragPosition = World * a_position;
