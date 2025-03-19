#version 150
uniform mat4 WorldViewProj;
uniform mat3 WorldIT;
in vec4 a_position;
in vec2 a_uv1;
in vec3 a_normal;
out vec2 v_uv1;
out vec3 v_normal;

void main()
{
    gl_Position = WorldViewProj * a_position;
	v_uv1 = a_uv1;
	v_normal = WorldIT * a_normal;
}
