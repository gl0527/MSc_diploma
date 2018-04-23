#version 130

varying vec2 diffuseMapCoord;
varying vec2 lightMapCoord;

uniform sampler2D diffuseMap;
uniform sampler2D lightMap;

void main()
{	
	gl_FragColor = texture2D (diffuseMap, diffuseMapCoord) * texture2D (lightMap, lightMapCoord);
}