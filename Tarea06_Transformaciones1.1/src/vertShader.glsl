#version 330                                                                  
                                                                              
layout (location = 0) in vec3 pos;	
layout (location = 1) in vec3 vColor;	

uniform float rotate; 
uniform mat4 mv;

const float radius = 0.2;
const float pi = 3.141592653589793;
const float twicePi = 2*pi;
const float numberOfSides = 50;
                                                           
out vec4 vertexColor;                                                                              
float toRadians = 3.14159265f / 180.0f;


// funtions to Transformations(Rotate, Translate, Scale)
mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);
mat4 buildScale(float sx, float sy, float sz);                    
                                                                              
void main()                                                                   
{   
	if(gl_InstanceID==1){ 
	                                                               
	float x_vertex =-1;
	float y_vertex = -0.9; 
	mat4 localRotaZ =  buildRotateZ(rotate *10* twicePi*2 / 360);
	
	//All Transformation funtions. 	
	mat4 localTrans = buildTranslate(x_vertex, y_vertex, 0); //Translate in x and y axis.
	mat4 localScal = buildScale(rotate/240+1,rotate/240+1, 0); //Scale in 20%.
	localTrans +=buildTranslate(rotate/180,0,0);
	gl_Position = localTrans * localRotaZ * localScal * vec4(pos.x,pos.y,pos.z, 1.0);  // right-to-left
	
	  vertexColor = vec4(vColor,1.0);	
    }
    else{


	
	gl_Position = mv*vec4(pos.x,pos.y,pos.z, 1.0);
    vertexColor = vec4(vColor,1.0);	
	}
    
       
}

// builds and returns a matrix that performs a rotation around the X axis
mat4 buildRotateX(float rad) {
    mat4 xrot = mat4(1.0, 0.0,      0.0,       0.0,
                     0.0, cos(rad), -sin(rad), 0.0,
                     0.0, sin(rad), cos(rad),  0.0,
                     0.0, 0.0,      0.0,       1.0);
    return xrot;
}

// builds and returns a matrix that performs a rotation around the Y axis
mat4 buildRotateY(float rad) {
    mat4 yrot = mat4(cos(rad),  0.0, sin(rad), 0.0,
                     0.0,       1.0, 0.0,      0.0,
                     -sin(rad), 0.0, cos(rad), 0.0,
                     0.0,       0.0, 0.0,      1.0);
    return yrot;
}

// builds and returns a matrix that performs a rotation around the Z axis
mat4 buildRotateZ(float rad) {
    mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
                     sin(rad), cos(rad),  0.0, 0.0,
                     0.0,      0.0,       1.0, 0.0,
                     0.0,      0.0,       0.0, 1.0);
    return zrot;
}

// builds and returns a translation matrix
mat4 buildTranslate(float x, float y, float z) {
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      x,   y,   z,   1.0);
    return trans;
}

// builds and returns a Scale matrix
mat4 buildScale(float sx, float sy, float sz) {
    mat4 scale = mat4( sx, 0.0, 0.0, 0.0,
                      0.0,  sy, 0.0, 0.0,
                      0.0, 0.0,  sz, 0.0,
                      0.0, 0.0, 0.0, 1.0);
    return scale;
}
