#version 330 core
// FRAGMENT SHADER

smooth in vec2 texCoord;
uniform sampler2D tex;

void main() {
    gl_FragColor = texture2D(tex, texCoord);
}
