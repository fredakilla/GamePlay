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



void main()
{

	//Ambient = vec4(0.2,0.2,0.2,1.0);

	//vec4 Color = texture2D(s_texColor, v_texcoord0);

	
	// step1 : no color
	/*
	gl_FragColor = Color;
	*/


	// step2 : ambient lighting
	/*
	vec3 scatteredLight = vec3(Ambient); // this is the only light
	vec3 rgb = min(Color.rgb * scatteredLight, vec3(1.0));
	gl_FragColor = vec4(rgb, Color.a);
	*/


	// step3 : directionnal light
/*
	vec3 LightColor = vec3(1.0, 0.2, 1.0);
	vec3 LightDirection = vec3(0.0, 0.1, 0.1);
	vec3 HalfVector = vec3(0.0, 0.1, 0.1);
	float Shininess = 0.25;
	float Strength = 1.0;

	// compute cosine of the directions, using dot products,
	// to see how much light would be reflected
	float diffuse = max(0.0, dot(v_normal, LightDirection));
	float specular = max(0.0, dot(v_normal, HalfVector));

	// surfaces facing away from the light (negative dot products)
	// won’t be lit by the directional light
	if (diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess); // sharpen the highlight

	vec3 scatteredLight = Ambient.rgb + LightColor * diffuse;
	vec3 reflectedLight = LightColor * specular * Strength;

	// don’t modulate the underlying color with reflected light,
	// only with scattered light
	vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0));
	gl_FragColor = vec4(rgb, Color.a);	
	*/




	vec3 lightColor = vec3(1.0, 0.2, 1.0);
	vec3 lightDir = vec3(0.0, 1.0, 0.0);
	vec4 objectColor = texture2D(s_texColor, v_texcoord0);

	/*float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * objectColor.rgb;
    gl_FragColor = vec4(result, 1.0);*/


    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(v_normal);
    //vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec3 result = (ambient + diffuse) * objectColor.rgb;
	gl_FragColor = vec4(result, 1.0);

}
