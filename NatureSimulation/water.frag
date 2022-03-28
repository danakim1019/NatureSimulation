#version 450 //indicating opengl version

in vec4 clipSpace;

in vec3 fcolors;

out vec4 FragColors;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;


void main()
{
		vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0+0.5;
		vec2 refractTexCoords = vec2(ndc.x,ndc.y);
		vec2 reflectTexCoords = vec2(ndc.x,-ndc.y);

		vec4 reflectColour = texture(reflectionTexture, reflectTexCoords);
		vec4 refractColour = texture(refractionTexture, refractTexCoords);

		FragColors = mix(reflectColour,refractColour,0.5);
}