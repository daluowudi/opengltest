attribute vec4 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoord;

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform mat4 billBoardMatrix;

uniform vec3 cameraPos;
const vec3 up = vec3( 0.0, 1.0, 0.0 );

mat4 BillboardMatrix( void )
{
    vec3 forward = normalize( a_position.xyz - cameraPos );
    vec3 side = cross( forward, up );
    
    mat4 result;
    result[0] = vec4( side, 0.0 );
    result[1] = vec4( up, 0.0 );
    result[2] = vec4( forward.x, 0.0, forward.z, 0.0 );
    result[3] = vec4( 0.0, 0.0, 0.0, 1.0 );
    return result;
}

void main()
{
//    gl_Position = CC_MVPMatrix *billBoardMatrix * vec4(a_position.xyz,1);
    
    gl_Position = CC_MVPMatrix *BillboardMatrix() * vec4(a_position.xyz,1);
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
}
