#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D u_staticTex;
uniform sampler2D u_dynamicTex;
uniform sampler2D u_normalTex;
uniform sampler2D u_occlusionTex;

void main()
{
    vec4 staticColor = texture2D(u_staticTex, v_texCoord);
    vec4 dynamicColor = texture2D(u_dynamicTex, v_texCoord);
    vec4 normalColor = texture2D(u_normalTex, v_texCoord);
    vec4 occlusionColor = texture2D(u_occlusionTex, v_texCoord);
    
    gl_FragColor = v_fragmentColor * vec4(0.0, 0.0, 1.0, 1.0);
}

