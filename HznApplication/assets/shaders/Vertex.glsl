#version 420 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec4 a_Color;

uniform mat4 u_Projection;
uniform mat4 u_View;

out vec4 v_Color;

void main()
{
	gl_Position = u_Projection * u_View * vec4(a_Pos, 1.0f);
	v_Color = a_Color;
}