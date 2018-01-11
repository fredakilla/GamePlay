$input v_color0, v_texcoord0

#include "common/common.sh"

SAMPLER2D(u_texture, 0);
uniform vec2 u_cutoff;

void main()
{
	gl_FragColor = v_color0;
    float distance = texture2D(u_texture, v_texcoord0).r;
    float smoothing = fwidth(distance);
    float alpha = smoothstep(0.5 - smoothing * u_cutoff.x, 0.5 + smoothing * u_cutoff.y, distance);
    gl_FragColor.a = alpha * v_color0.a;
}
