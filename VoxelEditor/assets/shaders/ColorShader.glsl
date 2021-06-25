// Basic Flat Color Shader

#type vertex
#version 330 core	

layout(location = 0) in vec3 position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Position;
	
void main() {
	v_Position = position;
	gl_Position = u_ViewProjection * u_Transform * vec4(position, 1.0);
}


#type fragment
#version 330 core	

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

in vec3 v_Position;
			
void main() {
	color = u_Color;
}