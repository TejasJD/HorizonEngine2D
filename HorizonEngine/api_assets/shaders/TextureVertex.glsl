#version 420 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in int a_TexSlot;
layout (location = 4) in int a_EntityId;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 v_Color;
out vec2 v_TexCoord;
out flat int v_TexSlot;
out flat int v_EntityId;
		
void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexSlot = a_TexSlot;
	v_EntityId = a_EntityId;
	gl_Position = u_Projection * u_View * vec4(a_Pos, 1.0f);
}