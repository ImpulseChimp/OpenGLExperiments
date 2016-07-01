#version 430

in vec4 gl_FragCoord;
in vec2 gl_PointCoord;
out vec4 fColor;

float rand(vec2 n)
{
  return 0.5 + 0.5 * fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

void
main()
{
	float x = gl_PointCoord[0];
	fColor = vec4(x, 0.8, 1.0, 1.0);
}