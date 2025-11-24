#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D cloudTexture;
uniform vec3 viewPos;
uniform float time;
uniform vec3 skyColor; // Sky color for matching cloud color to time of day

void main()
{
    // Sample cloud texture
    vec4 texColor = texture(cloudTexture, TexCoords);
    
    // Use luminance as alpha for soft transparency
    float luminance = texColor.r; // Grayscale texture, so R = G = B
    
    // Soft alpha falloff - makes edges very soft
    float alpha = smoothstep(0.1, 0.7, luminance);
    
    // Cloud color changes with time of day
    // During day: pure white
    // During night: dark gray/black to match sky
    vec3 dayCloudColor = vec3(1.0, 1.0, 1.0);
    vec3 nightCloudColor = skyColor * 0.3; // Slightly lighter than sky but still dark
    
    // Blend based on sky brightness (simple heuristic)
    float skyBrightness = (skyColor.r + skyColor.g + skyColor.b) / 3.0;
    vec3 cloudColor = mix(nightCloudColor, dayCloudColor, smoothstep(0.1, 0.4, skyBrightness));
    
    // Subtle lighting based on view angle (rim lighting effect)
    vec3 viewDir = normalize(viewPos - FragPos);
    float rim = 1.0 - max(dot(normalize(Normal), viewDir), 0.0);
    rim = smoothstep(0.6, 1.0, rim);
    
    // Add subtle brightness variation based on rim (less pronounced at night)
    cloudColor += rim * 0.15 * skyBrightness;
    
    // Optional: Add very subtle time-based shimmer (only during day)
    float shimmer = sin(time * 0.5 + FragPos.x * 0.1 + FragPos.z * 0.1) * 0.02 + 1.0;
    cloudColor *= mix(1.0, shimmer, skyBrightness);
    
    // Clamp to prevent over-brightness
    cloudColor = clamp(cloudColor, 0.0, 1.0);
    
    // Final color with alpha (more transparent at night)
    float finalAlpha = alpha * mix(0.6, 0.85, skyBrightness);
    FragColor = vec4(cloudColor, finalAlpha);
    
    // Discard very transparent fragments for performance
    if (FragColor.a < 0.01) {
        discard;
    }
}
