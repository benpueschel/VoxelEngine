// Basic Texture Shader

#type vertex
#version 450	

layout(location = 0) in vec3 _Position;
layout(location = 1) in vec4 _Color;
layout(location = 2) in vec2 _TexCoord;
layout(location = 3) in float _TexIndex;
layout(location = 4) in vec2 _TexScale;
layout(location = 5) in int _EntityID;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out vec2 v_TexScale;
out flat int v_EntityID;

void main() {
	v_TexCoord = _TexCoord;
	v_TexIndex = _TexIndex;
	v_TexScale = _TexScale;
	v_Color = _Color;
	v_EntityID = _EntityID;
	gl_Position = u_ViewProjection * vec4(_Position, 1.0);
}

#type fragment
#version 450

layout(location = 0) out vec4 color;
layout(location = 1) out int entityID;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in vec2 v_TexScale;
in flat int v_EntityID;

uniform sampler2D u_Textures[32];
			
void main() {
	vec4 texColor = v_Color;

	switch(int(v_TexIndex)) {
		case 0: texColor *= texture(u_Textures[0], v_TexCoord * v_TexScale); break;
		case 1: texColor *= texture(u_Textures[1], v_TexCoord * v_TexScale); break;
		case 2: texColor *= texture(u_Textures[2], v_TexCoord * v_TexScale); break;
		case 3: texColor *= texture(u_Textures[3], v_TexCoord * v_TexScale); break;
		case 4: texColor *= texture(u_Textures[4], v_TexCoord * v_TexScale); break;
		case 5: texColor *= texture(u_Textures[5], v_TexCoord * v_TexScale); break;
		case 6: texColor *= texture(u_Textures[6], v_TexCoord * v_TexScale); break;
		case 7: texColor *= texture(u_Textures[7], v_TexCoord * v_TexScale); break;
		case 8: texColor *= texture(u_Textures[8], v_TexCoord * v_TexScale); break;
		case 9: texColor *= texture(u_Textures[9], v_TexCoord * v_TexScale); break;
		case 10: texColor *= texture(u_Textures[10], v_TexCoord * v_TexScale); break;
		case 11: texColor *= texture(u_Textures[11], v_TexCoord * v_TexScale); break;
		case 12: texColor *= texture(u_Textures[12], v_TexCoord * v_TexScale); break;
		case 13: texColor *= texture(u_Textures[13], v_TexCoord * v_TexScale); break;
		case 14: texColor *= texture(u_Textures[14], v_TexCoord * v_TexScale); break;
		case 15: texColor *= texture(u_Textures[15], v_TexCoord * v_TexScale); break;
		case 16: texColor *= texture(u_Textures[16], v_TexCoord * v_TexScale); break;
		case 17: texColor *= texture(u_Textures[17], v_TexCoord * v_TexScale); break;
		case 18: texColor *= texture(u_Textures[18], v_TexCoord * v_TexScale); break;
		case 19: texColor *= texture(u_Textures[19], v_TexCoord * v_TexScale); break;
		case 20: texColor *= texture(u_Textures[20], v_TexCoord * v_TexScale); break;
		case 21: texColor *= texture(u_Textures[21], v_TexCoord * v_TexScale); break;
		case 22: texColor *= texture(u_Textures[22], v_TexCoord * v_TexScale); break;
		case 23: texColor *= texture(u_Textures[23], v_TexCoord * v_TexScale); break;
		case 24: texColor *= texture(u_Textures[24], v_TexCoord * v_TexScale); break;
		case 25: texColor *= texture(u_Textures[25], v_TexCoord * v_TexScale); break;
		case 26: texColor *= texture(u_Textures[26], v_TexCoord * v_TexScale); break;
		case 27: texColor *= texture(u_Textures[27], v_TexCoord * v_TexScale); break;
		case 28: texColor *= texture(u_Textures[28], v_TexCoord * v_TexScale); break;
		case 29: texColor *= texture(u_Textures[29], v_TexCoord * v_TexScale); break;
		case 30: texColor *= texture(u_Textures[30], v_TexCoord * v_TexScale); break;
		case 31: texColor *= texture(u_Textures[31], v_TexCoord * v_TexScale); break;
	}

	color = texColor;
	entityID = v_EntityID;
}