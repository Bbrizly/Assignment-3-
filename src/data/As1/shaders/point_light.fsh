#version 150
in vec2 v_uv1;
in vec3 v_normal;
in vec3 v_fragPosition;

uniform sampler2D texture1;
uniform float fade;

out vec4 PixelColor;

struct PointLight
{
    vec4 PositionRange; // .xyz is position, .w is range
    vec3 Color;
    float Strength;
};

uniform PointLight blueLight;   // Blue point light that follows the player
uniform PointLight yellowLight; // Yellow point light at the lamp post

// Function to calculate attenuation based on distance and light range
float calculateAttenuation(vec3 lightPos, float lightRange, vec3 fragPos)
{
    float distance = length(lightPos - fragPos);
    if (distance > lightRange)
        return 0.0; // No contribution if outside the light's range
    // Linear attenuation
    float attenuation = 1.0 - (distance / lightRange);
    return attenuation * attenuation; // Smooth shits
}

void main()
{
    vec3 normal = v_normal;//normalize(v_normal);
    vec3 totalLighting = vec3(0.2); // Base ambient lighting

    // Calculate contribution from the blue light
    float attenuation = calculateAttenuation(blueLight.PositionRange.xyz, blueLight.PositionRange.w, v_fragPosition);
    vec3 lightDir = normalize(blueLight.PositionRange.xyz - v_fragPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    totalLighting += attenuation * diff * blueLight.Color * blueLight.Strength;

    // Calculate contribution from the yellow light
    attenuation = calculateAttenuation(yellowLight.PositionRange.xyz, yellowLight.PositionRange.w, v_fragPosition);
    lightDir = normalize(yellowLight.PositionRange.xyz - v_fragPosition);
    diff = max(dot(normal, lightDir), 0.0);
    totalLighting += attenuation * diff * yellowLight.Color * yellowLight.Strength;

    // Sample the texture and apply the computed lighting
    vec4 texColor = texture(texture1, v_uv1);
    PixelColor = vec4(texColor.rgb * totalLighting, texColor.a * fade);
    //PixelColor = vec4(v_normal * 0.5 + 0.5, 1.0);
    //PixelColor = vec4(v_normal,1.0);
}
