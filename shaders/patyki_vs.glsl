#version 330
uniform mat4 modelViewProjectionMatrix;
layout( location = 0 ) in vec4 inPosition;
void main()
{
    gl_Position = modelViewProjectionMatrix * inPosition;
}
