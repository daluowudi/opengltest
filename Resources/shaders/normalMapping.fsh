varying vec2 v_texCoord;
//varying vec3 normal_cameraspace;
varying vec3 lightdirection_camera;
varying vec3 pos_world;
varying vec3 eyedirection_camera;

varying vec3 lightdirection_tangentspace;
varying vec3 eyedirection_tangentspace;

uniform vec3 u_lightpos_world;
uniform vec4 u_lightColor;
uniform float u_lightPower;
uniform float u_ambientlight;

uniform sampler2D u_diffuse_sampler;
uniform sampler2D u_normal_sampler;

void main()
{
    vec3 texturenormal_tangentspace = texture2D(u_normal_sampler, v_texCoord).rgb*2.0 - 1.0;
    vec3 n = normalize(texturenormal_tangentspace);
    vec3 l = normalize(lightdirection_tangentspace);
    float costheta = clamp(dot(n,l),0.0,1.0);
    // 镜面反射
    vec3 E = normalize(eyedirection_tangentspace);
    vec3 R = reflect(-l, n);
    float cosAlpha = clamp(dot(E, R), 0.0, 1.0);
    
    // 纹理颜色
    vec4 texColor = texture2D(u_diffuse_sampler, v_texCoord);
//    CC_Texture1
    // 反射点颜色
    vec4 specularColor = texColor;//vec4(0.3,0.3,0.3,1);
    float distance = length(u_lightpos_world - pos_world);
    
    // 加入一点背光(模拟光照后带来反射影响模型背面,防止模型非见光面全黑)
    vec4 backlightcolor = texColor * u_ambientlight;
    
//    gl_FragColor = vec4((specularColor * u_lightColor * u_lightPower * pow(cosAlpha, 5.0) / (distance * distance)  + backlightcolor ).rgb, 1);
    gl_FragColor = vec4((backlightcolor +
                        texColor * u_lightColor * u_lightPower * costheta / (distance * distance)).rgb, 1);
}