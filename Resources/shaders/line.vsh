attribute vec3 a_position;
attribute vec4 a_color;

void main(){
    gl_Position = a_position;
    v_fragmentColor = a_color;
}