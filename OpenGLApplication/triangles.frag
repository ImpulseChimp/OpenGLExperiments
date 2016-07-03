#version 430

out vec4 fColor;

in float xLoc;
in float yLoc;
in float zLoc;

void
main()
{
	fColor = vec4(xLoc + 0.5f, yLoc + 0.25f, zLoc, 1.0);
}