#version 430

out vec4 fColor;

in float xLoc;
in float yLoc;
in float zLoc;

void
main()
{
	fColor = vec4(sin(xLoc), cos(yLoc), zLoc, 1.0);
}