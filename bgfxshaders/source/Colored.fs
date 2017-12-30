$input v_color0

#include "common/common.sh"


uniform vec4 u_diffuseColor;

void main()
{
	gl_FragColor = v_color0;
	gl_FragColor.x = u_diffuseColor.x;
}