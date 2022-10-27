# version 420 core
		
in vec2 v_TexCoord;
out vec4 v_Color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
	v_Color = texture(u_Texture, v_TexCoord) * u_Color;
}