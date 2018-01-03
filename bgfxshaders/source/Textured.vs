$input a_position, a_normal, a_texcoord0
$output v_texcoord0

#include "common/common.sh"

#if defined(TEXTURE_REPEAT)
uniform vec4 u_textureRepeat;
#endif

void main()
{
	gl_Position = mul(u_worldViewProjectionMatrix, vec4(a_position, 1.0) );
	v_texcoord0 = a_texcoord0;

	#if defined(TEXTURE_REPEAT)
    v_texcoord0 *= vec2(u_textureRepeat.x, u_textureRepeat.y);
    #endif
}