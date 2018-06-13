varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
varying vec3 normal_cameraspace;
varying vec3 lightdirection_camera;
varying vec3 pos_world;
varying vec3 lightpos_world;

vec4 lightColor;
float lightPower;

void main()
{
//    gl_FragColor = v_fragmentColor;
//    gl_FragColor = texture2D(CC_Texture0, v_texCoord);
    
    vec3 n = normalize(normal_cameraspace);
    vec3 l = normalize(lightdirection_camera);
    float cosTheta = clamp(dot(n,l),0.0,1.0);
    
    lightColor = vec4(1.0,1.0,1.0,1.0);
    lightPower = 60.0;
    float distance = length(lightpos_world - pos_world);
    
    gl_FragColor = v_fragmentColor * lightColor * lightPower * cosTheta / (distance * distance);
}