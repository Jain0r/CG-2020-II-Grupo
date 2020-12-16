#version 330 core
in vec4 vertexColor;
out vec4 fColor;

void main()
{  
	fColor = vertexColor; 	
		
	vec4 Marron =vec4(0.533f, 0.443f, 0.105f,1.0f);
	
	float y =gl_FragCoord.y-400;
	float x =gl_FragCoord.x-400;
	
	//y = y -30;
	
	//if(x<15&& y<15 && x>0 && y>0 ){
	
	//fColor = Marron;
	//}
}