attribute vec4 a_position;
attribute vec4 a_color;

varying vec4 v_fragmentColor;

void main()
{
    gl_Position = CC_MVPMatrix * a_position;
//    gl_Position = vec4(a_position.xyz,1);
    v_fragmentColor = a_color;
}