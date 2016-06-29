#version 430

layout (location = 0) in vec4 vPosition;
layout (location = 6) in vec4 vOffset;

void 
main()
{
    vPosition[0] += offset[0];
    vPosition[1] += offset[0];
	gl_Position = vPosition;
}