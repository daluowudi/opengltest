varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()
{
    gl_FragColor = v_fragmentColor;
//    gl_FragColor = texture2D(CC_Texture0, v_texCoord);
}