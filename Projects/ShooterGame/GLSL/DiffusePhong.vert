#version 130

uniform mat4 modelMatrix;
uniform mat4 modelMatrixInverse;
uniform mat4 modelViewProjMatrix;

varying vec2 diffuseMapCoord;
varying vec2 lightMapCoord;
varying vec2 ambientMapCoord;
varying vec3 worldNorm;
varying vec4 worldPos;

void main ()
{
   gl_Position = modelViewProjMatrix * gl_Vertex;
   worldPos = modelMatrix * gl_Vertex;
   worldNorm = vec3 (vec4 (gl_Normal, 0.0) * modelMatrixInverse);
   diffuseMapCoord = gl_MultiTexCoord0.xy;
   lightMapCoord = gl_MultiTexCoord1.xy;
   ambientMapCoord = worldPos.xz;
   ambientMapCoord += vec2 (170.0, 255.0);
   ambientMapCoord /= vec2 (340.0, 425.0);
}