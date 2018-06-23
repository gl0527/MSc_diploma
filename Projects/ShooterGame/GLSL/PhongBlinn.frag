#version 130

varying vec4 worldPos;
varying vec3 worldNorm;

uniform vec4 ks;
uniform vec4 eyePos;
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
	vec3 n_toEye = normalize (eyePos.xyz - worldPos.xyz);
	vec3 n_halfway = normalize (n_toEye + n_toLight);
	float NdotH = dot (n_normal, n_halfway);
	if (NdotH < 0) {
		gl_FragColor = vec4 (0.0, 0.0, 0.0, 1.0);
		return;
	}
	float lightDist = length (toLight);
	float attenuation = 1.0 / (lightAttenuation.y + lightAttenuation.z * lightDist + lightAttenuation.w * lightDist * lightDist);
	gl_FragColor = vec4 (lightPowerDensity * attenuation * ks.xyz * pow (NdotH, ks.w), 1.0);
}