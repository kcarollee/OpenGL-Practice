#version 330 core
out vec4 FragColor;
in vec3 vertexColor; // input variable from vs (same name and type)
uniform vec4 newColor;

void main()
{
	FragColor = vec4(vertexColor, 1.0);
}