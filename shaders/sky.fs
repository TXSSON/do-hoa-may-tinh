#version 330 core
out vec4 FragColor;

in vec3 LocalPos;

uniform vec3 topColor;
uniform vec3 bottomColor;
uniform float time;
uniform bool isNight;

// Simple hash function for pseudo-random stars
float hash(vec3 p) {
    p = fract(p * 0.3183099 + 0.1);
    p *= 17.0;
    return fract(p.x * p.y * p.z * (p.x + p.y + p.z));
}

void main()
{
    vec3 dir = normalize(LocalPos);
    float t = 0.5 * (dir.y + 1.0); // Map -1..1 to 0..1
    // Tweak gradient to be more realistic (more blue at top, white at horizon)
    t = smoothstep(0.0, 0.8, dir.y); 
    
    vec3 skyColor = mix(bottomColor, topColor, t);
    
    // Add stars at night
    if (isNight && dir.y > 0.0) { // Only above horizon
        // Create star field using hash function
        vec3 starPos = dir * 100.0; // Scale up for more stars
        vec3 gridPos = floor(starPos);
        
        float starHash = hash(gridPos);
        
        // Only some grid cells have stars (threshold)
        if (starHash > 0.97) { // 3% of cells have stars
            // Star brightness with twinkling
            float twinkle = sin(time * 2.0 + starHash * 100.0) * 0.3 + 0.7;
            float brightness = (starHash - 0.97) / 0.03; // 0 to 1
            brightness *= twinkle;
            
            // Distance from grid center (for star size)
            vec3 localPos = fract(starPos);
            float dist = length(localPos - 0.5);
            
            // Small star size
            float starSize = 0.02 + brightness * 0.03;
            if (dist < starSize) {
                float starIntensity = (1.0 - dist / starSize) * brightness;
                skyColor += vec3(starIntensity * 0.8, starIntensity * 0.8, starIntensity);
            }
        }
    }
    
    FragColor = vec4(skyColor, 1.0);
}
