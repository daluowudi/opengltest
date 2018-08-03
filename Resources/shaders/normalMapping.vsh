attribute vec3 a_position;
attribute vec2 a_texCoord;
attribute vec3 a_vertexNormal;
attribute vec3 a_vertexTangent;
attribute vec3 a_vertexBitangent;

varying vec2 v_texCoord;
varying vec3 normal_cameraspace;
varying vec3 lightdirection_camera;
varying vec3 pos_world;
varying vec3 eyedirection_camera;


uniform vec3 u_lightpos_world;
uniform mat3 MV3x3;

void main()
{
    gl_Position = CC_MVPMatrix * vec4(a_position.xyz,1);
    
    v_texCoord = a_texCoord;
    
    pos_world = a_position;
    vec3 pos_camera = (CC_MVMatrix * vec4(a_position, 1)).xyz;
    eyedirection_camera = vec3(0,0,0) - pos_camera;
    
    vec3 lightpos_cameraspace = (CC_MVMatrix * vec4(u_lightpos_world,1)).xyz; // mvmatrix == vmatrix
    lightdirection_camera = lightpos_cameraspace + eyedirection_camera;
    
    normal_cameraspace = (CC_MVMatrix * vec4(a_vertexNormal,0)).xyz;
}