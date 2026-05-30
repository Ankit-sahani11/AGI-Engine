#version 450

// Array size 3 hona chahiye kyunki 3 points hain
vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

void main() {
    // gl_Position set karna zaroori hai
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}

