#version 130

varying vec2 diffuseMapCoord;
varying vec2 ambientMapCoord;

uniform sampler2D diffuseMap;
uniform sampler2D ambientMap;

void main()
{
	gl_FragColor = 	texture2D (diffuseMap, diffuseMapCoord) *
					texture2D (ambientMap, ambientMapCoord);
}