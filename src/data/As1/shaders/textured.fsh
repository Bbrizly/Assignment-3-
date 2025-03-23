#version 150
in vec2 v_uv1;
in vec3 v_normal;
in vec3 v_fragPosition;

uniform sampler2D texture1;
uniform float fade;

out vec4 PixelColor;


struct PointLight
{
    vec3 Position;
    vec3 Color;
    float Strength;
};

uniform PointLight blueLight;   // Blue point light that follows the player
uniform PointLight yellowLight; // Yellow point light at the lamp post

void main()
{
    vec3 normal = normalize(v_normal);
    vec3 totalLighting = vec3(0.4); // Base ambient lighting

    // Calculate contribution from the blue light
    vec3 lightDir = normalize(blueLight.Position - v_fragPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    totalLighting += diff * blueLight.Color;

    // Calculate contribution from the yellow light
    lightDir = normalize(yellowLight.Position - v_fragPosition);
    diff = max(dot(normal, lightDir), 0.0);
    totalLighting += diff * yellowLight.Color;

    // Sample the texture and apply the computed lighting
    vec4 texColor = texture(texture1, v_uv1);
    PixelColor = vec4(texColor.rgb * totalLighting, texColor.a * fade);



    //PixelColor = texture(texture1, v_uv1);
    //PixelColor = vec4(v_normal,1.0);
	//PixelColor.a *= fade;

}



/*
#version 150

in vec2 v_uv1;
in vec3 v_normal;
in vec3 v_fragPosition;

uniform sampler2D texture1;
uniform float fade;

// Point Light Struct
struct PointLight
{
    vec3 Position;
    vec3 Color;
};

uniform PointLight blueLight;   // Blue point light that follows the player
uniform PointLight yellowLight; // Yellow point light at the lamp post

out vec4 PixelColor;
void main()
{
    vec3 normal = normalize(v_normal);
    vec3 totalLighting = vec3(0.1); // Base ambient lighting

    // Calculate contribution from the blue light
    vec3 lightDir = normalize(blueLight.Position - v_fragPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    totalLighting += diff * blueLight.Color;

    // Calculate contribution from the yellow light
    lightDir = normalize(yellowLight.Position - v_fragPosition);
    diff = max(dot(normal, lightDir), 0.0);
    totalLighting += diff * yellowLight.Color;

    // Sample the texture and apply the computed lighting
    vec4 texColor = texture(texture1, v_uv1);
    PixelColor = vec4(texColor.rgb * totalLighting, texColor.a * fade);
}
*/

/*
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
*/