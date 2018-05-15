#version 400

out vec4 FragColor;

in vec3 Pos;
in vec3 Norm;

uniform vec3 camPos;
uniform samplerCube skybox;

void main()
{
	float ratio = 1.00/2.42;
	vec3 I = normalize(Pos - camPos);
	vec3 R = refract(I, normalize(Norm), ratio);
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
