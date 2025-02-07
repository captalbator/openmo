#version 400 core
out vec4 fragColor;

in vec4 fragPos;
flat in vec3 fragNormal;
in vec2 texCoords;

uniform vec3 uViewPos;

uniform sampler2D uTexture;

void main() {
	vec4 tex = texture(uTexture, texCoords);
	vec3 texColor = tex.rgb;

	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * texColor;
	vec3 lightDir = normalize(uViewPos - fragPos.xyz);
	float diff = max(dot(normalize(fragNormal), lightDir), 0.0);
	vec3 diffuse = diff * texColor;

	float distance = length(fragPos.xyz - uViewPos) / 1600;
	vec3 result = (ambient + diffuse); // max(distance, 1);
  
  fragColor = vec4(texColor, 1.0);
  //fragColor = vec4(distance/1000);
}
