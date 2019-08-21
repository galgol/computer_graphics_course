/*#version 130

varying vec2 texCoord0;
varying vec3 color0;

//varying vec4 FragColor;

uniform sampler2D texture;

void main()
{
	
gl_FragColor =  texture2D( texture, texCoord0 );
} */
#version 130

varying vec2 texCoord0;
//varying vec3 normal0;
varying vec3 color0;

uniform vec3 lightDirection;
uniform sampler2D texture;

void main()
{
  
	 gl_FragColor =  texture2D (texture, texCoord0);

}