$input a_position, a_texcoord0, a_color0
$output v_texcoord0
$output v_color0

#include "common/common.sh"

uniform mat4 u_projectionMatrix;

void main()
{
	gl_Position = mul(u_projectionMatrix, vec4(a_position, 1.0) );
	v_texcoord0 = a_texcoord0;
	v_color0 = a_color0;
}
