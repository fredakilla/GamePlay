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
















struct LightInfo
{
	vec3 Position;	//Light Position in eye-coords
	vec3 La;		//Ambient light intensity
	vec3 Ld;		//Diffuse light intensity
	vec3 Ls;		//Specular light intensity
};


struct MaterialInfo
{
	vec3 Ka;			//Ambient reflectivity
	vec3 Kd;			//Diffuse reflectivity
	vec3 Ks;			//Specular reflectivity
	float Shininess;	//Specular shininess factor
};




#define LIGHTCOUNT 1
LightInfo Light[LIGHTCOUNT];
MaterialInfo Material;









void light( int lightIndex, vec3 position, vec3 norm, out vec3 ambient, out vec3 diffuse, out vec3 spec )
{
	vec3 n = normalize( norm );
	vec3 s = normalize( Light[lightIndex].Position - position );
	vec3 v = normalize( -position );
	vec3 r = reflect( -s, n );
 
	ambient = Light[lightIndex].La * Material.Ka;
 
	float sDotN = max( dot( s, n ), 0.0 );
	diffuse = Light[lightIndex].Ld * Material.Kd * sDotN;
 
 
	spec = Light[lightIndex].Ls * Material.Ks * pow( max( dot(r,v) , 0.0 ), Material.Shininess ); 
}



void main()
{

/*

	Light[0].Position = vec3(0,1,0);
	Light[0].La = vec3(0.2,0.2,0.2);
	Light[0].Ld = vec3(0.8,0.8,0.8);
	Light[0].Ls = vec3(0.8,0.8,0.8);


    Material.Ka = vec3(0.5,0.5,0.5);
    Material.Kd = vec3(0.5,0.5,0.5);
    Material.Ks = vec3(0.5,0.5,0.5);
    Material.Shininess = 0.5f;

	
    vec3 ambientSum = vec3(0);
	vec3 diffuseSum = vec3(0);
	vec3 specSum = vec3(0);
	vec3 ambient, diffuse, spec;

	



	if ( gl_FrontFacing )
	{
		for( int i=0; i<LIGHTCOUNT; ++i )
		{
			light( i, v_wpos, v_normal, ambient, diffuse, spec );
			ambientSum += ambient;
			diffuseSum += diffuse;
			specSum += spec;
		}
	}
	else
	{
		for( int i=0; i<LIGHTCOUNT; ++i )
		{
			light( i, v_wpos, v_normal, ambient, diffuse, spec );
			ambientSum += ambient;
			diffuseSum += diffuse;
			specSum += spec;
		}
	}

	ambientSum /= vec3(LIGHTCOUNT,LIGHTCOUNT,LIGHTCOUNT);
 
	vec4 texColor = texture2D(s_texColor, v_texcoord0);
 
	gl_FragColor = vec4( ambientSum + diffuseSum, 1 ) * texColor + vec4( specSum, 1 );


*/



	/*mat3 tbn = mat3(
				normalize(v_tangent),
				normalize(v_bitangent),
				normalize(v_normal)
				);

	vec3 normal;
	normal.xy = texture2D(s_texNormal, v_texcoord0).xy * 2.0 - 1.0;
	normal.z = sqrt(1.0 - dot(normal.xy, normal.xy) );
	vec3 view = -normalize(v_view);

	vec3 lightColor;
	lightColor =  calcLight(0, tbn, v_wpos, normal, view);
	lightColor += calcLight(1, tbn, v_wpos, normal, view);
	lightColor += calcLight(2, tbn, v_wpos, normal, view);
	lightColor += calcLight(3, tbn, v_wpos, normal, view);

	vec4 color = toLinear(texture2D(s_texColor, v_texcoord0) );

	gl_FragColor.xyz = max(vec3_splat(0.05), lightColor.xyz)*color.xyz;
	gl_FragColor.w = 1.0;
	gl_FragColor = toGamma(gl_FragColor);*/

	


	/*

	vec4 texColor = texture2D(s_texColor, v_texcoord0);


	vec3 lightDir = vec3(0.0, 0.0, -1.0);
	float ndotl = dot(normalize(v_normal), lightDir);
	float spec = pow(ndotl, 30.0);
	gl_FragColor = vec4(pow(pow(texColor.xyz, vec3_splat(2.2) ) * ndotl + spec, vec3_splat(1.0/2.2) ), 1.0);
*/


	vec4 texColor = texture2D(s_texColor, v_texcoord0);
	vec3 lightDir = vec3(0.0, 0.0, -1.0);

	vec3 N = normalize(v_normal);
	vec3 L = normalize(lightDir);	
	float lambertTerm = dot(N,L);


	//float ndotl = dot(normalize(v_normal), lightDir);
	//float spec = pow(ndotl, 30.0);
	//gl_FragColor = vec4(pow(pow(texColor.xyz, vec3_splat(2.2) ) * ndotl + spec, vec3_splat(1.0/2.2) ), 1.0);

}
