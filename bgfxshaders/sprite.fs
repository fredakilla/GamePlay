$input v_texcoord0
$input v_color0

#include "common/common.sh"

SAMPLER2D(u_texture, 0);

void main()
{
	gl_FragColor = v_color0 * texture2D(u_texture, v_texcoord0);
}