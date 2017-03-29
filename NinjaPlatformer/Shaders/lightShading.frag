#version 330


in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

void main() {

	float distance = length(fragmentUV);
    color = vec4(fragmentColor.rgb, fragmentColor.a * (pow(0.05, distance) - 0.05));
}