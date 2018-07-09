attribute vec3 a_position;
attribute vec2 a_texCoord;
attribute vec3 a_vertexNormal;

varying vec2 v_texCoord;

void main()
{
	gl_Position = CC_MVPMatrix * vec4(a_position.xyz,1);
	v_texCoord = a_texCoord;
}