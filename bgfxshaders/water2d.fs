$input v_texcoord0
$input v_color0

#include "common/common.sh"

SAMPLER2D(u_texture, 0);
SAMPLER2D(u_texture_noise, 1);
uniform float u_time;

const float AMPLITUDE = 0.008;
const float FREQUENCY = 40.0;
const float SPEED = 0.5;

void main()
{
	vec2 displacement = texture2D (u_texture_noise, v_texcoord0 / 6.0).xy;
    float t = v_texcoord0.y + displacement.y * 0.1 - 0.10 + (sin (v_texcoord0.x * FREQUENCY + (u_time * SPEED)) * AMPLITUDE);
    gl_FragColor = v_color0 * texture2D(u_texture, vec2(v_texcoord0.x, t));
}
