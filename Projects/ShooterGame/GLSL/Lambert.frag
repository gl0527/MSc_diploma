#version 130

varying vec4 worldPos;
varying vec3 worldNorm;
varying vec2 diffuseMapCoord;

uniform sampler2D diffuseMap;
uniform vec4 lightPos;
uniform vec3 lightPowerDensity;
uniform vec4 lightAttenuation;

void main()
{
	vec3 n_normal = normalize (worldNorm);
	vec3 toLight = lightPos.xyz - worldPos.xyz;
	vec3 n_toLight = normalize (toLight);
	float NdotL = dot (n_normal, n_toLight);
	if (NdotL < 0) {
		gl_FragColor = vec4 (0.0, 0.0, 0.0, 1.0);
		return;
	}
	vec3 kd = texture2D (diffuseMap, diffuseMapCoord).xyz;
	float lightDist = length (toLight);
	float attenuation = 1.0 / (lightAttenuation.y + lightAttenuation.z * lightDist + lightAttenuation.w * lightDist * lightDist);
	gl_FragColor = vec4 (lightPowerDensity * attenuation * kd * NdotL, 1.0);
}