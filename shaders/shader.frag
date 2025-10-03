#version 460 core

in vec4 v_color;

layout(location = 0) out vec4 color;

layout(location = 0) uniform float u_Time;

void main() {
    vec4 c = v_color + vec4(1.1f, 1.2f, 1.3f, 1.0f) * u_Time * 0.1;
    color = c - ivec4(c);
}
