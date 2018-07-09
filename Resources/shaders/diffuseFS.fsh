varying vec2 v_texCoord;
varying vec3 normal_cameraspace;
varying vec3 lightdirection_camera;
varying vec3 pos_world;

uniform vec3 u_lightpos_world;
uniform vec4 u_lightColor;
uniform float u_lightPower;
uniform float u_ambientlight;

void main()
{
    vec3 n = normalize(normal_cameraspace);
    vec3 l = normalize(lightdirection_camera);
    float cosTheta = clamp(dot(n,l),0.0,1.0);
    
    float distance = length(u_lightpos_world - pos_world);
    
    // 纹理颜色
    vec4 texColor = texture2D(CC_Texture0, v_texCoord);
    
    vec4 mertialColor = texColor;
    
    // 加入一点背光(模拟光照后带来反射影响模型背面,防止模型非见光面全黑)
    vec4 backlightcolor = mertialColor * u_ambientlight;
    gl_FragColor = vec4((mertialColor * u_lightColor * u_lightPower * cosTheta / (distance * distance) + backlightcolor).rgb, 1);
}