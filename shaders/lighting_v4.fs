#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 18

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform sampler2D shadowMap;
uniform bool useLuminanceAlpha;
uniform bool isNight; // For building window lights
uniform float time;   // For window light variation
uniform bool enableWindowLights; // Only enable for buildings
uniform bool enableBulbGlow; // Enable emissive glow for street light bulbs
uniform vec3 objectColor = vec3(1.0); // Tint color (default white)

// Function prototypes
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// Simple hash for pseudo-random window lights
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

void main()
{    
    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Calculate Shadow
    vec3 lightDir = normalize(-dirLight.direction);
    float shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir);
    
    // Phase 1: Directional lighting (with shadow)
    vec3 result = CalcDirLight(dirLight, norm, viewDir, shadow);
    
    // Phase 2: Point lights (No shadow mapping for point lights in this version)
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    
    // Phase 3: Spot light (No shadow mapping for spot light in this version)
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    // Phase 4: Building window lights at night (emissive) - ONLY FOR BUILDINGS
    if (isNight && enableWindowLights) {
        // Create window grid pattern using texture coordinates
        // Reduced grid: 3 columns x 6 rows (less dense)
        vec2 windowGrid = fract(TexCoords * vec2(3.0, 6.0));
        
        // Window frame (dark borders) - larger borders for more spacing
        float windowFrame = step(0.15, windowGrid.x) * step(windowGrid.x, 0.85) *
                           step(0.15, windowGrid.y) * step(windowGrid.y, 0.85);
        
        // Random window on/off state (based on grid position)
        vec2 gridCell = floor(TexCoords * vec2(3.0, 6.0));
        float randomState = hash(gridCell);
        
        // Only 35% of windows are lit at night (more sparse)
        float isLit = step(0.65, randomState);
        
        // Slight flicker for realism
        float flicker = 0.9 + 0.1 * sin(time * 3.0 + randomState * 100.0);
        
        // Improved window light colors - more natural
        vec3 windowLight;
        if (randomState < 0.7) {
            // Warm incandescent (70%) - soft yellow
            windowLight = vec3(1.0, 0.8, 0.5);
        } else if (randomState < 0.9) {
            // Neutral white (20%) - LED/fluorescent
            windowLight = vec3(0.95, 0.95, 0.85);
        } else {
            // Cool white (10%) - modern LED
            windowLight = vec3(0.85, 0.9, 1.0);
        }
        windowLight *= 0.6; // Reduced intensity for subtlety
        
        // Add emissive light from windows
        float windowIntensity = windowFrame * isLit * flicker;
        result += windowLight * windowIntensity;
    }
    
    // Phase 5: Street light bulb glow at night (emissive)
    if (isNight && enableBulbGlow) {
        // Use objectColor for tinting (yellow for street lights, red for decorative)
        vec3 bulbGlow = objectColor; 
        result += bulbGlow * 3.5; // Higher intensity for visibility
    }
    
    float alpha = 1.0;
    if (useLuminanceAlpha) {
        vec3 texColor = texture(material.diffuse, TexCoords).rgb;
        // Calculate luminance (brightness)
        float luminance = dot(texColor, vec3(0.299, 0.587, 0.114));
        // Use smoothstep to clip dark background (0.0-0.1) and smooth transition
        alpha = smoothstep(0.1, 0.6, luminance); 
    } else {
        alpha = texture(material.diffuse, TexCoords).a;
    }

    FragColor = vec4(result * objectColor, alpha);
}

// Shadow Calculation
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    
    // PCF (Percentage-closer filtering) for soft shadows
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.diffuse, TexCoords)); 
    
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.diffuse, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.diffuse, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
