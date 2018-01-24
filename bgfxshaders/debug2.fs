$input v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0 // in...

#include "common/common.sh"

SAMPLER2D(s_texColor,  0);
SAMPLER2D(s_texNormal, 1);
uniform vec4 u_lightPosRadius[4];
uniform vec4 u_lightRgbInnerR[4];

vec2 blinn(vec3 _lightDir, vec3 _normal, vec3 _viewDir)
{
	float ndotl = dot(_normal, _lightDir);
	vec3 reflected = _lightDir - 2.0*ndotl*_normal; // reflect(_lightDir, _normal);
	float rdotv = dot(reflected, _viewDir);
	return vec2(ndotl, rdotv);
}

float fresnel(float _ndotl, float _bias, float _pow)
{
	float facing = (1.0 - _ndotl);
	return max(_bias + (1.0 - _bias) * pow(facing, _pow), 0.0);
}

vec4 lit(float _ndotl, float _rdotv, float _m)
{
	float diff = max(0.0, _ndotl);
	float spec = step(0.0, _ndotl) * max(0.0, _rdotv * _m);
	return vec4(1.0, diff, spec, 1.0);
}

vec4 powRgba(vec4 _rgba, float _pow)
{
	vec4 result;
	result.xyz = pow(_rgba.xyz, vec3_splat(_pow) );
	result.w = _rgba.w;
	return result;
}

vec3 calcLight(int _idx, mat3 _tbn, vec3 _wpos, vec3 _normal, vec3 _view)
{
	vec3 lp = u_lightPosRadius[_idx].xyz - _wpos;
	float attn = 1.0 - smoothstep(u_lightRgbInnerR[_idx].w, 1.0, length(lp) / u_lightPosRadius[_idx].w);
	vec3 lightDir = mul(_tbn, normalize(lp) );
	vec2 bln = blinn(lightDir, _normal, _view);
	vec4 lc = lit(bln.x, bln.y, 1.0);
	vec3 rgb = u_lightRgbInnerR[_idx].xyz * saturate(lc.y) * attn;
	return rgb;
}









vec4 Ambient;

vec3 Light = vec3(0,0,1);

void main()
{

	vec4 color = texture2D(s_texColor, v_texcoord0);


	/*vec3 diffuse = vec3(0.5f, 0.5f, 0.5f);
	vec3 ambient = color.rgb; //vec3(0.1, 0.0, 0.0);
	vec3 rgb = ambient + diffuse * saturate(dot(Light, v_normal));
	gl_FragColor = vec4(rgb, 1.0);*/

/*
	vec3 lightDir = vec3(0.0, -0.5, 0.2);
	float ndotl = dot(normalize(v_normal), -lightDir);
	float spec = pow(ndotl, 30.0);
	gl_FragColor = vec4(pow(pow(color.xyz, vec3_splat(2.2) ) * ndotl + spec, vec3_splat(1.0/2.2) ), 1.0);
	*/




	vec3 lightColor = vec3(0.7, 0.7, 0.7);
	vec3 lightDir = vec3(0, 1, 0);

	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(v_normal);
    //vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec3 result = (ambient + diffuse) * color.rgb;
	gl_FragColor = vec4(result, 1.0);

}
