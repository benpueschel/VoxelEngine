// Basic Texture Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 _Position;
layout(location = 1) in vec4 _Color;
layout(location = 2) in vec2 _TexCoord;
layout(location = 3) in float _TexIndex;
layout(location = 4) in vec2 _TexScale;
layout(location = 5) in int _EntityID;

layout(std140, binding = 0) uniform Camera 
{
	mat4 u_ViewProjection;
};

struct VertexData 
{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
	vec2 TexScale;
};

layout(location = 0) out VertexData Data;
layout(location = 4) out flat int v_EntityID;

void main() {
	Data.TexCoord = _TexCoord;
	Data.TexIndex = _TexIndex;
	Data.TexScale = _TexScale;
	Data.Color = _Color;
	v_EntityID = _EntityID;
	gl_Position = u_ViewProjection * vec4(_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int entityID;

struct VertexData 
{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
	vec2 TexScale;
};

layout(location = 0) in VertexData Data;
layout(location = 4) in flat int v_EntityID;

layout(binding = 0) uniform sampler2D u_Textures[32];
			
void main() {
	vec4 texColor = Data.Color;

	switch(int(Data.TexIndex)) {
		case  0: texColor *= texture(u_Textures[0],  Data.TexCoord * Data.TexScale); break;
		case  1: texColor *= texture(u_Textures[1],  Data.TexCoord * Data.TexScale); break;
		case  2: texColor *= texture(u_Textures[2],  Data.TexCoord * Data.TexScale); break;
		case  3: texColor *= texture(u_Textures[3],  Data.TexCoord * Data.TexScale); break;
		case  4: texColor *= texture(u_Textures[4],  Data.TexCoord * Data.TexScale); break;
		case  5: texColor *= texture(u_Textures[5],  Data.TexCoord * Data.TexScale); break;
		case  6: texColor *= texture(u_Textures[6],  Data.TexCoord * Data.TexScale); break;
		case  7: texColor *= texture(u_Textures[7],  Data.TexCoord * Data.TexScale); break;
		case  8: texColor *= texture(u_Textures[8],  Data.TexCoord * Data.TexScale); break;
		case  9: texColor *= texture(u_Textures[9],  Data.TexCoord * Data.TexScale); break;
		case 10: texColor *= texture(u_Textures[10], Data.TexCoord * Data.TexScale); break;
		case 11: texColor *= texture(u_Textures[11], Data.TexCoord * Data.TexScale); break;
		case 12: texColor *= texture(u_Textures[12], Data.TexCoord * Data.TexScale); break;
		case 13: texColor *= texture(u_Textures[13], Data.TexCoord * Data.TexScale); break;
		case 14: texColor *= texture(u_Textures[14], Data.TexCoord * Data.TexScale); break;
		case 15: texColor *= texture(u_Textures[15], Data.TexCoord * Data.TexScale); break;
		case 16: texColor *= texture(u_Textures[16], Data.TexCoord * Data.TexScale); break;
		case 17: texColor *= texture(u_Textures[17], Data.TexCoord * Data.TexScale); break;
		case 18: texColor *= texture(u_Textures[18], Data.TexCoord * Data.TexScale); break;
		case 19: texColor *= texture(u_Textures[19], Data.TexCoord * Data.TexScale); break;
		case 20: texColor *= texture(u_Textures[20], Data.TexCoord * Data.TexScale); break;
		case 21: texColor *= texture(u_Textures[21], Data.TexCoord * Data.TexScale); break;
		case 22: texColor *= texture(u_Textures[22], Data.TexCoord * Data.TexScale); break;
		case 23: texColor *= texture(u_Textures[23], Data.TexCoord * Data.TexScale); break;
		case 24: texColor *= texture(u_Textures[24], Data.TexCoord * Data.TexScale); break;
		case 25: texColor *= texture(u_Textures[25], Data.TexCoord * Data.TexScale); break;
		case 26: texColor *= texture(u_Textures[26], Data.TexCoord * Data.TexScale); break;
		case 27: texColor *= texture(u_Textures[27], Data.TexCoord * Data.TexScale); break;
		case 28: texColor *= texture(u_Textures[28], Data.TexCoord * Data.TexScale); break;
		case 29: texColor *= texture(u_Textures[29], Data.TexCoord * Data.TexScale); break;
		case 30: texColor *= texture(u_Textures[30], Data.TexCoord * Data.TexScale); break;
		case 31: texColor *= texture(u_Textures[31], Data.TexCoord * Data.TexScale); break;
	}

	color = texColor;
	entityID = v_EntityID;
}