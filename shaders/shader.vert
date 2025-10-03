#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

layout(location = 0) uniform float u_Time;

out vec4 v_color;

void main() {
    v_color = color;

    gl_Position = position + vec4(sin(u_Time) * 0.1, cos(u_Time) * 0.1, 0.0, 0.0);
}
