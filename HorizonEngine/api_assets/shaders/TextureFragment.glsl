# version 420 core

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexSlot;

out vec4 f_Color;

uniform sampler2D u_Textures[32];

void main()
{
	f_Color = texture(u_Textures[int(v_TexSlot)], v_TexCoord) * v_Color;                                               
}