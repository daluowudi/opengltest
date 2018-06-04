varying vec4 v_fragmentColor;
uniform vec4 u_color;
varying vec2 v_texCoord;

void main()
{
//    gl_FragColor = v_fragmentColor * u_color;
//    gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
    gl_FragColor = texture2D(CC_Texture0, v_texCoord);
//    gl_FragColor = v_fragmentColor;
}