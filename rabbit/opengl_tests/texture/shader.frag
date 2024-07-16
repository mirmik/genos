#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    color = texture(ourTexture2, TexCoord);
	if(color.a < 0.1)
        discard;
    //mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 1);
}