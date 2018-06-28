varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
varying vec3 normal_cameraspace;
varying vec3 lightdirection_camera;
varying vec3 pos_world;
varying vec3 eyedirection_camera;

uniform vec3 u_lightpos_world;
uniform vec4 u_lightColor;
uniform float u_lightPower;
uniform float u_ambientlight;

void main()
{
//    gl_FragColor = texture2D(CC_Texture0, v_texCoord);
    vec3 n = normalize(normal_cameraspace);
    vec3 l = normalize(lightdirection_camera);
    float cosTheta = clamp(dot(n,l),0.0,1.0);
    
    // 镜面反射
    vec3 E = normalize(eyedirection_camera);
    vec3 R = reflect(-l, n);
    float cosAlpha = clamp(dot(E, R), 0.0, 1.0);
    // 反射点颜色
    vec4 specularColor = vec4(0.3,0.3,0.3,1);
    
    float distance = length(u_lightpos_world - pos_world);
    
    // 加入一点背光(模拟光照后带来反射影响模型背面,防止模型非见光面全黑)
    vec4 backlightcolor = v_fragmentColor * u_ambientlight;
    gl_FragColor = vec4((v_fragmentColor * u_lightColor * u_lightPower * cosTheta / (distance * distance) + backlightcolor).rgb, 1);
    
//    gl_FragColor = vec4((specularColor * u_lightColor * u_lightPower * pow(cosAlpha, 5.0) / (distance * distance)  + backlightcolor ).rgb, 1);
}