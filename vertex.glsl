#version 330 core

layout (location = 0) in vec2 position;

uniform float PointSize;

void main() {
    gl_PointSize = PointSize;
    gl_Position = vec4(position, 0.0f, 1.0f);
}
