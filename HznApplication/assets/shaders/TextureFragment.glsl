# version 420 core
		
in vec2 v_TexCoord;
out vec4 fragmentColor;

uniform sampler2D f_Texture;

void main()
{
	fragmentColor = texture(f_Texture, v_TexCoord);
}