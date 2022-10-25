#version 420 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 v_TexCoord;
		
void main()
{
	gl_Position = projection * view * model * vec4(a_Pos, 1.0f);
	v_TexCoord = a_TexCoord;
}