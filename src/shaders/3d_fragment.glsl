#version 460 core
#define MAX_LIGHTS 32

// uniform int lightCount;
// uniform vec3 lightPos[MAX_LIGHTS];
// uniform vec3 lightCol[MAX_LIGHTS];
uniform vec3 objCol;
uniform uint hasTexture;
uniform sampler2D diffuseTex;
uniform int shadingMode;

in vec3 fragPos;
in vec2 TexCoord;

out vec4 FragColor;

struct Light{
    vec4 position;
    vec4 color;
};

layout(std430, binding = 0) buffer LightBuffer{Light lights[];};

void main()
{

    vec3 lightSum = vec3(0.0);
    vec4 sampleColor = vec4(0,0,0,0);

    if (shadingMode == 0){

        float ambientStrength = 0.2;

        for(int i = 0; i < lights.length(); i ++){

            vec3 normal = normalize(cross(dFdx(fragPos), dFdy(fragPos)));
            vec3 lightDir = normalize(lights[i].position.xyz - fragPos); 

            // Ambient is basically a min light value for the fragment
            vec3 ambient = ambientStrength * lights[i].color.xyz;
            // Diffuse is scaled based on how much the triangle is pointing at the light
            vec3 diffuse = max(dot(normal, lightDir), 0.0) * lights[i].color.xyz;

            // Sum light contributions from all lights
            lightSum += diffuse + ambient;
        }
    }
    else {
        lightSum = vec3(1.0);
    }

    // If the object does not have a texture than use its solid color
    if (hasTexture == 1u){
        sampleColor = texture(diffuseTex, TexCoord);
    }
    else {
        sampleColor = vec4(objCol,1.0);
    }

    // Multiply the light by the sample color to shade object
    FragColor = vec4(lightSum,1.0) * sampleColor;
}
