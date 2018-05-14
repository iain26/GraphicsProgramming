#version 400

uniform mat4 u_pm;
uniform mat4 u_vm;
uniform vec3 lightDir;
uniform vec3 InputColor;

uniform float maxDist;
uniform float minDist; 

uniform float zPos;
 
layout( location = 0 ) out vec4 fragcolor;
 
in vec3 v_norm;
uniform mat4 v_pos; 
 
vec3 ToonRim() {
 
	vec3 n = normalize(mat3(u_vm) * v_norm);      
	vec3 p = vec3((u_pm) * v_pos);               
	vec3 v = normalize(p);                        
	float vdn = 0.6 - max(dot(v, n), 0.0);       
	
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

  return vec3(smoothstep(0.4, 0.8, vdn)) + vec3(color.x,color.y,color.z);
}

void main(){
	float dist = abs(zPos);
	float factor = (maxDist - dist)/(maxDist - minDist);
	factor = clamp(factor, 0.0, 0.1);
	vec3 toonRimContrib = ToonRim();

  vec3 color = mix(InputColor, toonRimContrib, factor);
  fragcolor = vec4(color, 1.0);
}