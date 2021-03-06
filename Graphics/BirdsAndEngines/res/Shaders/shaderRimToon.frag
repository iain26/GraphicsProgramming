#version 400

uniform mat4 u_pm;
uniform mat4 u_vm;
uniform vec3 lightDir;
uniform vec3 InputColor;
 
layout( location = 0 ) out vec4 fragcolor;
 
in vec3 v_norm;
uniform mat4 v_pos; 
 
void main() {
 
	vec3 n = normalize(mat3(u_vm) * v_norm);      // convert normal to view space
	vec3 p = vec3((u_pm) * v_pos);                // position in clip space
	vec3 v = normalize(p);                        // normalised eye vector
	float vdn = 0.6 - max(dot(v, n), 0.0);        // the rim contribution
	
	float intensity;
	vec4 color;
	intensity = dot(lightDir,v_norm);

	if (intensity > 0.95)
		color = vec4(InputColor,1.0);
	else if (intensity > 0.5)
		color = vec4(InputColor/2,1.0);
	else if (intensity > 0.25)
		color = vec4(InputColor/4,1.0);
	else
		color = vec4(InputColor/8,1.0);

  fragcolor.a = 0.6;
  fragcolor.rgb = vec3(smoothstep(0.4, 1.3, vdn)) + vec3(color.x,color.y,color.z);
}