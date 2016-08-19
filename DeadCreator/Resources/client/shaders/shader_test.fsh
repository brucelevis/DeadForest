#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 u_origin;
uniform vec2 u_resolution;

uniform sampler2D u_staticTex;
uniform sampler2D u_dynamicTex;
uniform sampler2D u_normalTex;
uniform sampler2D u_occlusionTex;

void main()
{
    vec4 originColor = texture2D(CC_Texture0, v_texCoord);
    vec4 staticColor = texture2D(u_staticTex, v_texCoord);
    vec4 dynamicColor = texture2D(u_dynamicTex, v_texCoord);
    vec4 normalColor = texture2D(u_normalTex, v_texCoord);
    vec4 occlusionColor = texture2D(u_occlusionTex, v_texCoord);
    
    vec2 origin = (gl_FragCoord.xy - u_origin) / u_resolution;
    
    vec4 resultColor = vec4(0.2, 0.0, 0.0, 1.0);
    if ( origin.x > 0.2 ) resultColor = vec4(0.0, 0.0, 1.0, 1.0);
    
    gl_FragColor = staticColor;
}

// Shader from http://www.iquilezles.org/apps/shadertoy/

//#ifdef GL_ES
//precision highp float;
//#endif
//
//uniform vec2 center;
//uniform vec2 resolution;
//
//void main(void)
//{
//    float time = CC_Time[1];
//    vec2 p = 2.0 * (gl_FragCoord.xy - center.xy) / resolution.xy;
//
//    // animate
//    float tt = mod(time,2.0)/2.0;
//    float ss = pow(tt,.2)*0.5 + 0.5;
//    ss -= ss*0.2*sin(tt*6.2831*5.0)*exp(-tt*6.0);
//    p *= vec2(0.5,1.5) + ss*vec2(0.5,-0.5);
//
//
//    float a = atan(p.x,p.y)/3.141593;
//    float r = length(p);
//
//    // shape
//    float h = abs(a);
//    float d = (13.0*h - 22.0*h*h + 10.0*h*h*h)/(6.0-5.0*h);
//
//    // color
//    float f = step(r,d) * pow(1.0-r/d,0.25);
//
//    gl_FragColor = vec4(f,0.0,0.0,1.0);
//}