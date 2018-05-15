#version 400

out vec3 Pos;
out vec3 Norm;

in vec3 aPos;
in vec3 aNorm;

uniform mat4 vp;
uniform mat4 m;


void main()
{
	Norm = mat3(transpose(inverse(m))) * aNorm;
	Pos = vec3(m * vec4(aPos, 1.0));
	gl_Position = vp * m * vec4(aPos, 1.0);
}