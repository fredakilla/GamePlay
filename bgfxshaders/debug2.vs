$input a_position, a_normal, a_tangent, a_texcoord0
$output v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0

#include "common/common.sh"



uniform mat4 u_matrixModel;
uniform mat4 u_matrixView;
uniform mat4 u_matrixProj;
uniform mat4 u_matrixModelView;
uniform mat4 u_matrixModelViewProj;

uniform mat4 u_matrixNormal;


void main()
{
	gl_Position = mul(u_worldViewProjectionMatrix, vec4(a_position, 1.0) );
	v_texcoord0 = a_texcoord0;

	//v_normal = a_normal.xyz;
	//v_normal = mul(u_model[0], a_normal).xyz;
	//v_normal = mul(u_matrixModel, a_normal).rgb;	
	v_normal = mul(u_matrixNormal, a_normal).rgb;
}
