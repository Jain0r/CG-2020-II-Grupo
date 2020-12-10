#version 330 core
out vec4 fColor;
uniform float u_time;
#define PI 3.14159265359



float box(in vec2 _st, in vec2 _size){
    _size = vec2(0.5) - _size*0.5;
    vec2 uv = smoothstep(_size,
                        _size+vec2(0.001),
                        _st);
    uv *= smoothstep(_size,
                    _size+vec2(0.001),
                    vec2(1.0)-_st);
    return uv.x*uv.y;
}

float cross(in vec2 _st, float _size){
    return  box(_st, vec2(_size,_size/3.)) +
            box(_st, vec2(_size/3.,_size));
}
mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}
mat2 scale(vec2 _scale){
    return mat2(_scale.x,0.0,
                0.0,_scale.y);


}

void main(){
    vec2 u_resolution=vec2(800, 800);
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec2 a = gl_FragCoord.xy/u_resolution.xy;

    st.x *= u_resolution.x/u_resolution.y;

    vec3 color = vec3(0.);
    color = vec3(st.x,st.y,abs(sin(u_time)));

    fColor = vec4(color,1.0);
     st -= vec2(0.5);
    
    st = rotate2d( sin(u_time/2.)*PI ) * st;
    st = scale( vec2(sin(u_time)+2.0) ) * st;
    
    st += vec2(0.5);
    vec2 translate = vec2(cos(u_time),sin(u_time));
    
    st += translate*0.35;

    color += vec3(cross(st,0.25));
   

    fColor = vec4(color,1.0);
}