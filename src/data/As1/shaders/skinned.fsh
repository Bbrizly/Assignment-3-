#version 150

struct PointLight
{
    vec4 PositionRange; // .xyz is position, .w is range
    vec3 Color;
    float Strength;
};
uniform sampler2D texture1;
uniform float fade;
uniform PointLight blueLight;   // Blue point light that follows the player
uniform PointLight yellowLight; // Yellow point light at the lamp post

in vec2 v_uv1;
in vec3 v_normal;
in vec3 v_fragPosition;

out vec4 PixelColor;

float calculateAttenuation(float distance, float range)
{
    float attenuation = clamp(1.0 - distance / range, 0.0, 1.0);
    return attenuation * attenuation; // Smooth attenuation
}

void main()
{

	vec3 n = normalize( v_normal );
	vec3 totalLighting = vec3(0.2f);//LightAmbient;

    vec3 lightDir = blueLight.PositionRange.xyz - v_fragPosition;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);
    float diff = max(dot(n, lightDir), 0.0);
    float attenuation = calculateAttenuation(distance, blueLight.PositionRange.w);
    totalLighting += diff * blueLight.Color * attenuation * blueLight.Strength;

    // Calculate contribution from the yellow light
    lightDir = yellowLight.PositionRange.xyz - v_fragPosition;
    distance = length(lightDir);
    lightDir = normalize(lightDir);
    diff = max(dot(n, lightDir), 0.0);
    attenuation = calculateAttenuation(distance, yellowLight.PositionRange.w);
    totalLighting += diff * yellowLight.Color * attenuation * yellowLight.Strength;


    vec4 texColor = texture(texture1, v_uv1);
    PixelColor = vec4(texColor.rgb * totalLighting, texColor.a * fade);
}