#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main( void ) {
	
	vec2 uv = ( gl_FragCoord.xy * 2.0 - resolution.xy) / min(resolution.x, resolution.y);
	
	// dead web panel
	float d = step(sin((uv.y - time *0.1 + abs(uv.x) * 0.5) * 8.0), 0.0);
	gl_FragColor = vec4( d, d, 0.0, 1.0 );
}