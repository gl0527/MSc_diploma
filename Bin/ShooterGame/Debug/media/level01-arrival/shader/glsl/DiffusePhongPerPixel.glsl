#version 130

varying vec2 texCoord;
varying vec3 worldNorm;
varying vec4 worldPos;

uniform sampler2D diffuseTex;
uniform vec4 ambientLightColor;
uniform vec4 diffuseLightColor;
uniform vec4 specularLightColor;
uniform vec4 ambientMaterialColor;
uniform vec4 specularMaterialColor;
uniform vec4 cameraPosition;
uniform vec4 lightPosition;
uniform vec4 lightAttenuation;

void main()
{
	vec4 diffuseMaterialColor = texture2D (diffuseTex, texCoord);
	vec4 ambientTerm = ambientLightColor * ambientMaterialColor * diffuseMaterialColor;

	vec3 normal = normalize (worldNorm);
	vec3 toLight = normalize (lightPosition.xyz - worldPos.xyz);
	float NdotL = dot (normal, toLight);
	
	if (NdotL < 0) {
		gl_FragColor = ambientTerm;
		return;
	}
	
	vec3 toEye = normalize (cameraPosition.xyz - worldPos.xyz);
	vec3 halfway = normalize (toEye + toLight);
	float NdotH = dot (normal, halfway);
	
	if (NdotH < 0) {
		gl_FragColor = ambientTerm;
		return;
	}
	
	float lightDist = length (lightPosition.xyz - worldPos.xyz);
	float attenuation = 1.0 / (lightAttenuation.y + lightAttenuation.z * lightDist + lightAttenuation.w * lightDist * lightDist);
			 
	vec4 attenuatedDiffuseLightColor = diffuseLightColor * attenuation;
	vec4 attenuatedSpecularLightColor = specularLightColor * attenuation;
	
	vec4 diffuseLighting = attenuatedDiffuseLightColor * diffuseMaterialColor * NdotL;
	vec4 specularLighting = attenuatedSpecularLightColor * vec4 (specularMaterialColor.xyz, 1.0) * pow (NdotH, specularMaterialColor.w);
	
	gl_FragColor = ambientTerm + diffuseLighting + specularLighting;
	gl_FragColor.w = 1.0;
}