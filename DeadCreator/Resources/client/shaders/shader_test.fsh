#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

//uniform vec2 u_origin;
//uniform vec2 u_resolution;

//uniform sampler2D u_staticTex;
//uniform sampler2D u_dynamicTex;
//uniform sampler2D u_normalTex;
//uniform sampler2D u_occlusionTex;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

void main()
{
//    vec4 originColor = texture2D(CC_Texture0, v_texCoord);
//    vec4 staticColor = texture2D(u_staticTex, v_texCoord);
//    vec4 dynamicColor = texture2D(u_dynamicTex, v_texCoord);
//    vec4 normalColor = texture2D(u_normalTex, v_texCoord);
//    vec4 occlusionColor = texture2D(u_occlusionTex, v_texCoord);
    vec4 originColor = texture2D(CC_Texture0, v_texCoord);
    vec4 texture1 = texture2D(u_texture1, v_texCoord);
    vec4 texture2 = texture2D(u_texture2, v_texCoord);
    
    gl_FragColor = originColor;
}