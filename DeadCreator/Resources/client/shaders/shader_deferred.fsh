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

vec4 blur(sampler2D tex, vec2 p, float blurRadius, float sampleNum)
{
    vec4 col = vec4(0);
    vec2 unit = 1.0 / vec2(1136.0, 640.0);
    
    float r = blurRadius;
    float sampleStep = r / sampleNum;
    
    float count = 0.0;
    
    for(float x = -r; x < r; x += sampleStep)
    {
        for(float y = -r; y < r; y += sampleStep)
        {
            float weight = (r - abs(x)) * (r - abs(y));
            col += texture2D(tex, p + vec2(x * unit.x, y * unit.y)) * weight;
            count += weight;
        }
    }
    
    return col / count;
}

void main()
{
    vec4 originColor = texture2D(CC_Texture0, v_texCoord);
    vec4 staticColor = texture2D(u_staticTex, v_texCoord);
    vec4 dynamicColor = texture2D(u_dynamicTex, v_texCoord);
    vec4 normalColor = texture2D(u_normalTex, v_texCoord);
    vec4 occlusionColor = blur(u_occlusionTex, v_texCoord, 15.0 , 2.0);
    
    vec4 visibleStaticColor = staticColor * occlusionColor.r;
    vec4 unvisibleStaticColor = staticColor * (1.0 - occlusionColor.r);
    float graySacledColor =
    (unvisibleStaticColor.r * 0.2126) +
    (unvisibleStaticColor.g * 0.7152) +
    (unvisibleStaticColor.g * 0.0722);
    graySacledColor *= 0.8;
    unvisibleStaticColor = vec4(graySacledColor, graySacledColor, graySacledColor, 1.0);
    
    staticColor = visibleStaticColor + unvisibleStaticColor;
    dynamicColor = dynamicColor * occlusionColor.r;
    
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 1.0);
    resultColor += ((1.0 - dynamicColor.a) * staticColor);
    resultColor += dynamicColor;
    
    gl_FragColor = resultColor;
}