#version 330

in vec3 aPosition;
in vec3 aNormal;

out VS_OUT{
	vec3 normal;
}vs_out;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
	 gl_Position = m * v * p * vec4(aPosition, 1.0); 
    mat3 normalMatrix = mat3(transpose(inverse(m * v)));
    vs_out.normal = normalize(vec3(p * vec4(normalMatrix * aNormal, 0.0)));
}