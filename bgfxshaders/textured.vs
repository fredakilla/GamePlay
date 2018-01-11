$input a_position, a_normal, a_texcoord0
$output v_texcoord0

#include "common/common.sh"

void main()
{
	gl_Position = mul(u_worldViewProjectionMatrix, vec4(a_position, 1.0) );
	v_texcoord0 = a_texcoord0;
}