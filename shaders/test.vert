#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 in_tex_coord;

out vec2 tex_coord;

uniform mat4 camera_view;
uniform mat4 camera_projection;
uniform mat4 model;

void main()
{
    // gl_Position = vec4(aPos, 1.0);
    gl_Position = camera_projection * camera_view * model * vec4(aPos, 1.0);
    tex_coord = in_tex_coord;
}
