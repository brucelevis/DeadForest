#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

//uniform vec2 resolution;
//uniform float blurRadius;
//uniform float sampleNum;

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
    
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 1.0);
    resultColor += ((1.0 - dynamicColor.a) * staticColor);
    resultColor += dynamicColor;
    
    gl_FragColor = resultColor;
}