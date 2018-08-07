attribute vec3 a_position;
attribute vec2 a_texCoord;
attribute vec3 a_vertexNormal;
attribute vec3 a_vertexTangent;
attribute vec3 a_vertexBitangent;

varying vec2 v_texCoord;
//varying vec3 normal_cameraspace;
varying vec3 lightdirection_camera;
varying vec3 pos_world;
varying vec3 eyedirection_camera;

varying vec3 lightdirection_tangentspace;
varying vec3 eyedirection_tangentspace;

uniform vec3 u_lightpos_world;
uniform mat3 MV3x3;

mat3 transpose(mat3 mt) {
    vec3 i0 = mt[0];
    vec3 i1 = mt[1];
    vec3 i2 = mt[2];
    
    mat3 outmt = mat3(
                      vec3(i0.x, i1.x, i2.x),
                      vec3(i0.y, i1.y, i2.y),
                      vec3(i0.z, i1.z, i2.z)
                      );
    return outmt;
}

void main()
{
    gl_Position = CC_MVPMatrix * vec4(a_position.xyz,1);
    
    v_texCoord = a_texCoord;
    
    pos_world = a_position;
    vec3 pos_camera = (CC_MVMatrix * vec4(a_position, 1)).xyz;
    eyedirection_camera = vec3(0,0,0) - pos_camera;
    
    vec3 lightpos_cameraspace = (CC_MVMatrix * vec4(u_lightpos_world,1)).xyz; // mvmatrix == vmatrix
    lightdirection_camera = lightpos_cameraspace + eyedirection_camera;
    
//    normal_cameraspace = (CC_MVMatrix * vec4(a_vertexNormal,0)).xyz;
    vec3 vertexNormal_cameraspace = MV3x3 * a_vertexNormal;
    vec3 vertexTangent_cameraspace = MV3x3 * a_vertexTangent;
    vec3 vertexBitangent_cameraspace = MV3x3 * a_vertexBitangent;
    
    mat3 TBN = transpose(mat3(
                              vertexTangent_cameraspace,
                              vertexBitangent_cameraspace,
                              vertexNormal_cameraspace
                         ));
    lightdirection_tangentspace = TBN * lightdirection_camera;
    eyedirection_tangentspace = TBN * eyedirection_camera;
}