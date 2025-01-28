#version 330

in vec2 tex_coord;

out vec4 FragColor;

uniform sampler2D u_texture;

void main()
{
    FragColor = texture(u_texture, vec2(tex_coord.x, 1.0 - tex_coord.y));
}
