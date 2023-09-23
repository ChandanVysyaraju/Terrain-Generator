#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 color;
uniform vec3 light_color;

void main() {
    FragColor = vec4(vec3(100.0 / 255.0, 20.0 / 255.0, 100.0 / 255.0) + (color * light_color), 1.0);
}