#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    int enabled;
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fragPosition;
in vec3 fragNormal; 

out vec4 finalColor;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform float fogDensity;
uniform vec4 fogColor;
uniform float opacity;

void main()
{
    // ambient light is always present
    vec3 ambient = light.ambient * material.ambient;
    
    // If light is not enabled, only ambient light contributes
    if (light.enabled == 0) {
        finalColor = vec4(ambient, 1.0);
        return;
    }
  	
    // diffuse 
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(light.position - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    finalColor = vec4(result, opacity);

    // Fog calculation
    float dist = length(viewPos - fragPosition);

    // Exponential fog
    float fogFactor = 1.0/exp((dist*fogDensity)*(dist*fogDensity));
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    finalColor = mix(fogColor, finalColor, fogFactor);
} 