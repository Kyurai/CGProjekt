#version 130
// default130. vert : a simple vertex shader
attribute vec4 vert;
attribute vec4 texCoord;
attribute vec4 normCoord;
uniform mat4 modelMatrix;
uniform mat4 perspectiveMatrix;
uniform mat4 viewMatrix;

varying vec4 texC;
varying vec4 norC;
void main()
{


    texC = texCoord;
    norC = normCoord;
    gl_Position = perspectiveMatrix * viewMatrix * modelMatrix * vert;
}
