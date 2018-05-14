#version 330

in vec3 aPos;

out vec3 texCoord;

uniform mat4 proj_view;

void main()
{
	texCoord = aPos;
	gl_Position = proj_view * vec4(aPos.x, aPos.y + 3, aPos.z - 4, 1.0);
}