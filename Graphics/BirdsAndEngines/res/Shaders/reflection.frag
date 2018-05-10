#version 400

out vec4 FragColor;

in vec3 Pos;
in vec3 Norm;

uniform vec3 camPos;
uniform samplerCube skybox;

void main()
{
	vec3 I = normalize(Pos - camPos);
	vec3 R = reflect(I, normalize(Norm));
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
