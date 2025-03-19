#version 150
in vec2 v_uv1;

uniform sampler2D texture1;
uniform float fade;

out vec4 PixelColor;

void main()
{
    PixelColor = texture(texture1, v_uv1);
	PixelColor.a *= fade;
}
