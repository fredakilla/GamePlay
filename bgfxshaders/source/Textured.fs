$input v_texcoord0

#include "common/common.sh"

SAMPLER2D(u_diffuseTexture, 0);

void main()
{
	vec4 texColor = texture2D(u_diffuseTexture, v_texcoord0);
	gl_FragColor = texColor;
}