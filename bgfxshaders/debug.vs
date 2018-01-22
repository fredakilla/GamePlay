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


	gl_Position = mul(u_matrixModelViewProj, vec4(a_position, 1.0) );
	v_texcoord0 = a_texcoord0;


	
	v_wpos = vec3(u_matrixModelView * vec4(a_position, 1));
	v_texcoord0 = a_texcoord0;
	v_normal = mul(u_model[0], a_normal).xyz;
	//v_normal = normalize( mul(a_normal, u_matrixNormal)).xyz;







	/*vec3 wpos = mul(u_model[0], vec4(a_position, 1.0) ).xyz;
	gl_Position = mul(u_worldViewProjectionMatrix, vec4(wpos, 1.0) );
	
	vec4 normal = a_normal * 2.0 - 1.0;
	vec3 wnormal = mul(u_model[0], vec4(normal.xyz, 0.0) ).xyz;

	vec4 tangent = a_tangent * 2.0 - 1.0;
	vec3 wtangent = mul(u_model[0], vec4(tangent.xyz, 0.0) ).xyz;

	vec3 viewNormal = normalize(mul(u_view, vec4(wnormal, 0.0) ).xyz);
	vec3 viewTangent = normalize(mul(u_view, vec4(wtangent, 0.0) ).xyz);
	vec3 viewBitangent = cross(viewNormal, viewTangent) * tangent.w;
	mat3 tbn = mat3(viewTangent, viewBitangent, viewNormal);

	v_wpos = wpos;

	vec3 view = mul(u_view, vec4(wpos, 0.0) ).xyz;
	v_view = mul(view, tbn);

	v_normal = viewNormal;
	v_tangent = viewTangent;
	v_bitangent = viewBitangent;

	v_texcoord0 = a_texcoord0;*/
}
