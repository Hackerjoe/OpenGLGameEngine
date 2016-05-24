#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
out vec2 texcoords;


uniform uint faceNum;
uniform vec3 eyepos;

void main()
{
    texcoords = texcoord;
    vec4 pos = vec4(position,1);
    gl_Position = pos;
}
