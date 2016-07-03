#version 430

in vec4 vPosition;
in vec4 color;
uniform mat4 ortho;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out float xLoc;
out float yLoc;
out float zLoc;

void 
main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;
	xLoc = gl_Position.x;
	yLoc = gl_Position.y;
	zLoc = gl_Position.z;
}