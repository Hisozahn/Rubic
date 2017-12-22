#version 330

out vec4 FragColor;

struct Material {
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Pos;
in vec3 FragPos; 
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform sampler2D textureFront;
uniform sampler2D textureRight;
uniform sampler2D textureBack;
uniform sampler2D textureLeft;
uniform sampler2D textureTop;
uniform sampler2D textureBottom;

const float epsilon = 0.0001;
void main()
{
    vec3 rgb;
    if (abs(Pos.x - 1.0) < epsilon) {
        rgb = texture(textureRight, TexCoords).rgb;
    }
    else if (abs(Pos.x + 1.0) < epsilon) {
        rgb = texture(textureLeft, TexCoords).rgb;
    }
    else if (abs(Pos.z - 1.0) < epsilon) {
        rgb = texture(textureFront, TexCoords).rgb;
    }
    else if (abs(Pos.z + 1.0) < epsilon) {
        rgb = texture(textureBack, TexCoords).rgb;
    }
    else if (abs(Pos.y - 1.0) < epsilon) {
        rgb = texture(textureTop, TexCoords).rgb;
    }
    else if (abs(Pos.y + 1.0) < epsilon) {
        rgb = texture(textureBottom, TexCoords).rgb;
    }

    // ambient
    vec3 ambient = light.ambient * rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}