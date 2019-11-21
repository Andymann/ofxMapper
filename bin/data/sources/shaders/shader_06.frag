/*
 * Original shader from: https://www.shadertoy.com/view/3dKGR1
 */

#ifdef GL_ES
precision mediump float;
#endif

// glslsandbox uniforms
uniform float time;
uniform vec2 resolution;

// shadertoy emulation
#define iTime time
#define iResolution resolution

// --------[ Original ShaderToy begins here ]---------- //
//plento

vec2 R;
const float pi = 3.1415926;
float hsh(vec2 p)//Dave hoshkin hash ya
{
	vec3 p3  = fract(vec3(p.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);}
void mainImage( out vec4 f, in vec2 u )
{
    R = iResolution.xy;
    vec2 uv = vec2(u.xy - .5*R.xy)/R.y;
    vec3 col = vec3(1);
    
    float d = .75; // speed factor
    
    
    float m = exp(abs(sin(uv.x*2. -iTime*d)));
    
    //m = min(m,2.0); // makes flat planes
   // m = max(m,1.5);
    
    uv.y*=m; // remmapping y coords to the function
    
    uv.y+=iTime*.7;
    uv.x-=iTime*(d/2.);
    col*=(step(0., sin(-uv.x*2.)) + .3); // dark shadow every other crest
    uv.x*=(3./pi)*4.; 
    
    // id and repeated coord
	vec2 fuv = fract(uv*6.);
    vec2 id = floor(uv*6.);
    
    //checkerboard value
    float chk = mod(id.y+id.x,2.);
   
    // shading/color
    col*=mix(vec3(1., 0., .5), vec3(0.,.6, .2), hsh(id));
    col *= smoothstep(.7, .27,abs(fuv.y-.5))*chk;
    col *= smoothstep(3.4, .9, m);
    
    f = vec4(col, 1.);
    
}
// --------[ Original ShaderToy ends here ]---------- //

void main(void)
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
}