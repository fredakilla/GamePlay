$input v_color0, v_texcoord0

#include "common/common.sh"

SAMPLER2D(u_texture, 0);

void main()
{
	gl_FragColor = v_color0;
    gl_FragColor.a = texture2D(u_texture, v_texcoord0).r * v_color0.a;
}
