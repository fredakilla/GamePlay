$input v_texcoord0

#define TEXTURE_DISCARD_ALPHA

#include "common/common.sh"

SAMPLER2D(u_diffuseTexture, 0);

void main()
{
	vec4 texColor = texture2D(u_diffuseTexture, v_texcoord0);
	gl_FragColor = texColor;

	#if defined(TEXTURE_DISCARD_ALPHA)
    if (gl_FragColor.a < 0.5)
        discard;
    #endif
    

}
