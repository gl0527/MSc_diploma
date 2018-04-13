#version 130

uniform mat4 inverseWorldMatrix;
uniform mat4 modelMatrix;

varying vec2 texCoord;
varying vec3 worldNorm;
varying vec4 worldPos;

void main ()
{
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   worldPos = modelMatrix * gl_Vertex;
   worldNorm = vec3 (vec4 (gl_Normal, 0.0) * inverseWorldMatrix);
   texCoord = gl_MultiTexCoord0.st;
}