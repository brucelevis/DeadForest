#ifdef GL_ES
precision lowp float;
#endif

varying vec2 v_texCoord;

// textures
uniform sampler2D u_normals;
uniform sampler2D u_dynamicMap;
uniform sampler2D u_staticMap;
uniform vec2 u_contentSize;

// light0's uniform variable
uniform int u_lightType0;
uniform vec3 u_lightColor0;
uniform float u_isEnable0;
uniform float u_intensity0;
uniform vec3 u_lightDir0;
uniform vec3 u_lightPos0;
uniform float u_lightRange0;

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

void main(void)
{
    vec4 fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 color  = texture2D(CC_Texture0, v_texCoord);
    vec4 normalMap = texture2D(u_normals, v_texCoord);
    vec4 dynamicMap = texture2D(u_dynamicMap, v_texCoord);
    vec4 staticMap = texture2D(u_staticMap, v_texCoord);
    vec3 pixelPos = vec3(v_texCoord.x * u_contentSize.x, (1.0 - v_texCoord.y) * u_contentSize.y ,0.0);
    vec3 eyePos = vec3(u_contentSize.x / 2.0, u_contentSize.y / 2.0, 100.0);
    
    
    // Light0
    if ( u_isEnable0 == 1.0 )
    {
        if ( u_lightType0 == 1 ) // directional light
        {
            vec3 ambient = calcAmbient(dynamicMap, u_lightColor0, u_intensity0);
            vec3 diffuse = calcDiffuse(dynamicMap, u_lightColor0, u_intensity0, normalMap, u_lightDir0);
            vec3 specular = calcSpecular(dynamicMap, u_lightColor0, u_intensity0, normalMap, u_lightDir0, eyePos, pixelPos);
            
            fragColor.rgb += (ambient + diffuse + specular);
        }
        else if ( u_lightType0 == 2 ) // point light
        {
            float dist = length(u_lightPos0 - pixelPos);
            float t = min(dist / u_lightRange0, 1.0);
            float identity = 1.0 - t;
            
            vec3 lightDir = normalize(pixelPos - u_lightPos0);
            vec3 ambient = calcAmbient(dynamicMap, u_lightColor0, u_intensity0);
            vec3 diffuse = calcDiffuse(dynamicMap, u_lightColor0, u_intensity0, normalMap, lightDir);
            vec3 specular = calcSpecular(dynamicMap, u_lightColor0, u_intensity0, normalMap, lightDir, eyePos, pixelPos);
            
            fragColor.rgb += (ambient + diffuse + specular) * identity;
        }
    }
    
    gl_FragColor = vec4(fragColor.rgb, color.a);
}
