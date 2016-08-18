#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D u_texture1;

void main()
{
    vec4 color = texture2D(u_texture1, v_texCoord);
    gl_FragColor = v_fragmentColor * color;
}

