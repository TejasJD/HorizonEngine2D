# version 420 core
layout (location = 0) out vec4 f_Color1;
layout (location = 1) out vec4 f_Color2;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexSlot;

uniform sampler2D u_Textures[32];

void main()
{
	f_Color1 = texture(u_Textures[int(v_TexSlot)], v_TexCoord) * v_Color;
	f_Color2 = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}