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

vec3 calcAmbient(in vec4 color, in vec3 lightColor, in float intensity)
{
    return vec3(color.rgb * lightColor * intensity);
}

vec3 calcDiffuse(in vec4 color, in vec3 lightColor, in float intensity, in vec4 normalMap,  in vec3 lightDir)
{
    float d = max(dot(-lightDir, normalMap.rgb), 0.0);
    return vec3(color.rgb * lightColor * intensity * d);
}

vec3 calcSpecular(in vec4 color,
                  in vec3 lightColor,
                  in float intensity,
                  in vec4 normalMap,
                  in vec3 lightDir,
                  in vec3 eyePos,
                  in vec3 pixelPos)
{
    vec3 v = normalize(eyePos - pixelPos);
    vec3 h = normalize(-lightDir + v);
    float s = pow(max(dot(normalMap.rgb, h), 0.0), 8.0);
    float d = max(dot(-lightDir, normalMap.rgb), 0.0);
    if ( d <= 0.0) s = 0.0;
    return vec3(color.rgb * lightColor * intensity * s);
}

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
    vec4 occlusionColor = blur(u_occlusionTex, v_texCoord, 15.0 , 3.0);
    
    vec4 visibleStaticColor = staticColor * occlusionColor.r;
    vec4 unvisibleStaticColor = staticColor * (1.0 - occlusionColor.r);
    float graySacledColor =
    (unvisibleStaticColor.r * 0.2126) +
    (unvisibleStaticColor.g * 0.7152) +
    (unvisibleStaticColor.g * 0.0722);
    unvisibleStaticColor = vec4(graySacledColor, graySacledColor, graySacledColor, 1.0);
    
    staticColor = visibleStaticColor + unvisibleStaticColor;
    dynamicColor = dynamicColor * occlusionColor.r;
    
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 1.0);
    resultColor += ((1.0 - dynamicColor.a) * staticColor);
    resultColor += dynamicColor;
    
    vec3 lightPos = vec3(568.0, 320.0, 100.0);
    vec3 pixelPos = vec3(v_texCoord.x * 1136.0, (1.0 - v_texCoord.y) * 640.0, 0.0);
    vec3 lightColor = vec3(1.0);
    float intensity = 1.0;
    float lightRange = 600.0;
    
    float dist = length(lightPos - pixelPos);
    float t = min(dist / lightRange, 1.0);
    float identity = 1.0 - t;
    
    vec3 lightDir = normalize(pixelPos - lightPos);
    vec3 ambient = calcAmbient(resultColor, lightColor, intensity);
    vec3 diffuse = calcDiffuse(resultColor, lightColor, intensity, normalColor, lightDir);
    vec3 specular = calcSpecular(resultColor, lightColor, intensity, normalColor, lightDir, lightPos, pixelPos);

    resultColor.rgb += (ambient + diffuse + specular) * identity * occlusionColor.r;
    
    gl_FragColor = resultColor * identity;
}