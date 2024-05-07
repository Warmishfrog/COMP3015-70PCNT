#version 460

in vec2 TexCoord;

uniform sampler2D SpriteTex;

layout (location = 0) out vec4 FragColor;

void main()
{
    // Sample the texture
    vec4 texColor = texture(SpriteTex, TexCoord);

    // Discard fragments with alpha below a certain threshold
    if (texColor.a < 0.2) discard;

    // Output the color with applied texture
    FragColor = texColor;
}
