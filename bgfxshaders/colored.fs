$input v_color0

#include "common/common.sh"

uniform vec4 u_diffuseColor;
vec4 _baseColor;

void main()
{
	#if defined(VERTEX_COLOR)
		_baseColor = v_color0;
    #else
    	_baseColor = u_diffuseColor;
	#endif

	gl_FragColor = _baseColor;
}