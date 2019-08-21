#version 130

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;

uniform vec3 lightDirection;
uniform vec3 lightColor;

void main()
{

	gl_FragColor = vec4(lightColor,1.0f) ;
}
