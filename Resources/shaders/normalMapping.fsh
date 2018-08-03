varying vec2 v_texCoord;
varying vec3 normal_cameraspace;
varying vec3 lightdirection_camera;
varying vec3 pos_world;
varying vec3 eyedirection_camera;

uniform vec3 u_lightpos_world;
uniform vec4 u_lightColor;
uniform float u_lightPower;
uniform float u_ambientlight;
//uniform sampler2D u_diffuse_sampler;
//uniform sampler2D u_normal_sampler;

void main()
{
    vec3 n = normalize(normal_cameraspace);
    vec3 l = normalize(lightdirection_camera);
    // 镜面反射
    vec3 E = normalize(eyedirection_camera);
    vec3 R = reflect(-l, n);
    float cosAlpha = clamp(dot(E, R), 0.0, 1.0);
    
    // 纹理颜色
    vec4 texColor = texture2D(CC_Texture0, v_texCoord);
//    CC_Texture1
    // 反射点颜色
    vec4 specularColor = texColor;//vec4(0.3,0.3,0.3,1);
    float distance = length(u_lightpos_world - pos_world);
    
    // 加入一点背光(模拟光照后带来反射影响模型背面,防止模型非见光面全黑)
    vec4 backlightcolor = texColor * u_ambientlight;
    
    gl_FragColor = vec4((specularColor * u_lightColor * u_lightPower * pow(cosAlpha, 5.0) / (distance * distance)  + backlightcolor ).rgb, 1);
}