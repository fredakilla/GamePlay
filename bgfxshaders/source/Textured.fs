$input v_color0, v_texcoord0

#include "common/common.sh"

SAMPLER2D(s_texColor, 0);
uniform vec4 u_diffuseColor;

void main()
{
	vec4 texColor = texture2D(s_texColor, v_texcoord0);

	gl_FragColor = v_color0 * texColor;
	gl_FragColor.x = u_diffuseColor.x;
}