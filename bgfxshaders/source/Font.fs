$input v_color0, v_texcoord0

#include "common/common.sh"

SAMPLER2D(u_texture, 0);
//uniform vec4 u_diffuseColor;

void main()
{
	///vec4 texColor = texture2D(u_texture, v_texcoord0);

	//gl_FragColor = v_color0 * texColor;
	//gl_FragColor.x = u_diffuseColor.x;*/

	gl_FragColor = v_color0;
    gl_FragColor.a = texture2D(u_texture, v_texcoord0).a * v_color0.a;
}