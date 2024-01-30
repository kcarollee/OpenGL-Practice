#version 330 core
out vec4 FragColor;
in vec3 vertexColor; // input variable from vs (same name and type)
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float lerpVal;

void main()
{
	// texture function: samples the corresponding color value using the texture parameters
	vec2 uv = TexCoord.xy;
	//uv.x = sin(uv. * 10.0);

	vec3 tColor1 = texture(texture1, uv).rgb;
	vec3 tColor2 = texture(texture2, uv).rgb;
	vec3 mixed = mix(tColor1, tColor2, lerpVal);
	FragColor = vec4(mixed, 1.0);
}