// Include standard headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

using namespace std;
// Vertex Array Object
GLuint VAO[14];

//  Vertex Buffer Object
GLuint VBO[14];

int dimVertices;
int numberOfVertices;
int numberOfVertices2;
int dimVertices2;

void init(){


	// Create vertices Contorno de cara
	GLfloat x = 0.5;
	GLfloat y = 0.5;
	GLfloat z = 0;
	GLfloat radius = 0.3;
	GLint numberOfSides = 50; // 50
	numberOfVertices = numberOfSides+2; // points + one center point.
	GLfloat twicePi = 2.0f * M_PI;

	GLfloat verticesX[numberOfVertices];
	GLfloat verticesY[numberOfVertices];
	GLfloat verticesZ[numberOfVertices];

	/*verticesX[0] = x;
	verticesY[0] = y;*/
	verticesZ[0] = z;

	for (int i = 0; i < numberOfVertices; i++) { //i=1 i<numberOf
		verticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		verticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		verticesZ[i] = z;
	}

	dimVertices = (numberOfVertices) * 3;

	GLfloat m_Vertices[dimVertices];

	for (int i = 0; i < numberOfVertices; i++) {
		m_Vertices[i * 3] = verticesX[i];
		m_Vertices[i * 3 + 1] = verticesY[i];
		m_Vertices[i * 3 + 2] = verticesZ[i];
	}

	//VERTICES DE LOS OJOS

	radius=0.03;
	for (int i = 0; i < numberOfVertices; i++) {
		verticesX[i] = 0.40 + (radius * cos(i * twicePi / numberOfSides));
		verticesY[i] = 0.60 + (radius * sin(i * twicePi / numberOfSides));
		verticesZ[i] = z;
	}
	GLfloat m_Vertices2[dimVertices];

	for (int i = 0; i < numberOfVertices; i++) {
		m_Vertices2[i * 3] = verticesX[i];
		m_Vertices2[i * 3 + 1] = verticesY[i];
		m_Vertices2[i * 3 + 2] = verticesZ[i];
	}

	radius=0.03;
	for (int i = 0; i < numberOfVertices; i++) {
		verticesX[i] = 0.60 + (radius * cos(i * twicePi / numberOfSides));
		verticesY[i] = 0.60 + (radius * sin(i * twicePi / numberOfSides));
		verticesZ[i] = z;
	}

	GLfloat m_Vertices3[dimVertices];

	for (int i = 0; i < numberOfVertices; i++) {
		m_Vertices3[i * 3] = verticesX[i];
		m_Vertices3[i * 3 + 1] = verticesY[i];
		m_Vertices3[i * 3 + 2] = verticesZ[i];
	}
	//VERTICE DE SONRISA


	radius=0.15;
	GLfloat radius2 = 0.08;
	for (int i = 0; i < numberOfVertices; i++) {
		verticesX[i] = x - (radius * cos(i * twicePi / numberOfSides));
		verticesY[i] = 0.4 - (radius2 * sin(i * twicePi / numberOfSides));
		verticesZ[i] = z;
	}

	GLfloat m_Vertices4[dimVertices];


	for (int i = 0; i < numberOfVertices; i++) {
		m_Vertices4[i * 3] = verticesX[i];
		m_Vertices4[i * 3 + 1] = verticesY[i];
	    m_Vertices4[i * 3 + 2] = verticesZ[i];
	}

	//VERTICES GATO
	//CUERPO
	x=-0.5;
	y=0.35;
	radius=0.25;
	for (int i = 0; i < numberOfVertices; i++) { //i=1 i<numberOf
		verticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		verticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		verticesZ[i] = z;
	}

	GLfloat m_Vertices5[dimVertices];
	for (int i = 0; i < numberOfVertices; i++) {
		m_Vertices5[i * 3] = verticesX[i];
		m_Vertices5[i * 3 + 1] = verticesY[i];
		m_Vertices5[i * 3 + 2] = verticesZ[i];
	}

	//CABEZA
	x=-0.5;
	y=0.75;
	radius=0.15;
	for (int i = 0; i < numberOfVertices; i++) { //i=1 i<numberOf
		verticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		verticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		verticesZ[i] = z;
	}

	GLfloat m_Vertices6[dimVertices];
	for (int i = 0; i < numberOfVertices; i++) {
		m_Vertices6[i * 3] = verticesX[i];
		m_Vertices6[i * 3 + 1] = verticesY[i];
		m_Vertices6[i * 3 + 2] = verticesZ[i];
	}

	//OREJA 1
	x=-0.37;
	y=0.86;
	radius=0.03;
	for (int i = 0; i < numberOfVertices; i++) { //i=1 i<numberOf
		verticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		verticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		verticesZ[i] = z;
	}

	GLfloat m_Vertices7[dimVertices];
	for (int i = 0; i < numberOfVertices; i++) {
		m_Vertices7[i * 3] = verticesX[i];
		m_Vertices7[i * 3 + 1] = verticesY[i];
		m_Vertices7[i * 3 + 2] = verticesZ[i];
	}

	//OREJA 2
	x=-0.63;
	y=0.86;
	radius=0.03;
	for (int i = 0; i < numberOfVertices; i++) { //i=1 i<numberOf
		verticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		verticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		verticesZ[i] = z;
	}

	GLfloat m_Vertices8[dimVertices];
	for (int i = 0; i < numberOfVertices; i++) {
		m_Vertices8[i * 3] = verticesX[i];
		m_Vertices8[i * 3 + 1] = verticesY[i];
		m_Vertices8[i * 3 + 2] = verticesZ[i];
	}

	//COLA
	x=-0.23;
	y=0.40;
	radius=0.2;
	for (int i = 0; i < numberOfVertices; i++) { //i=1 i<numberOf
		verticesX[i] = x + (radius * sin(i * twicePi / numberOfSides));
		verticesY[i] = y + (radius * cos(i * twicePi / numberOfSides));
		verticesZ[i] = z;
	}

	GLfloat m_Vertices9[dimVertices];
	for (int i = 0; i < numberOfVertices; i++) {
		m_Vertices9[i * 3] = verticesX[i];
		m_Vertices9[i * 3 + 1] = verticesY[i];
		m_Vertices9[i * 3 + 2] = verticesZ[i];
	}


	//CAPA_INVISIBLE
	radius=0.2;
	for (int i = 0; i < numberOfVertices; i++) {
		verticesX[i] = -0.5 + (radius * cos(i * twicePi / numberOfSides));
		verticesY[i] = -0.5 + (radius * sin(i * twicePi / numberOfSides));
		verticesZ[i] = z;
	}

	dimVertices = (numberOfVertices) * 3;

	GLfloat m_Vertices10[dimVertices];

	for (int i = 0; i < numberOfVertices; i++) {
		m_Vertices10[i * 3] = verticesX[i];
		m_Vertices10[i * 3 + 1] = verticesY[i];
		m_Vertices10[i * 3 + 2] = verticesZ[i];
	}

	//PIEDRA_RESURRECCION
	GLfloat m_Vertices11[]={
		-0.17f,-0.7f,0.0f,
		-0.83f,-0.7,0.0f,
		-0.5f,-0.05f,0.0f
	};

	//VARITA_ZAUCO
	GLfloat m_Vertices12[]={
		-0.5f,-0.7f,0.0f,
		-0.5f,-0.05f,0.0f
	};

	//LINEA DIVISION
	GLfloat m_Vertices13[]={
		-1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f
	};


	//GENERANDO PUNTOS

	glGenVertexArrays(14, VAO);
	glGenBuffers(14, VBO);

	//CONTORNO
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, dimVertices * sizeof(GLfloat), m_Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//OJOS

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices2), m_Vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,(void*) 0 );
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//OJOS2

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices3), m_Vertices3, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,	0, (void*) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//SONRISA

	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, dimVertices * sizeof(GLfloat), m_Vertices4, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  0,  (void*) 0 );

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//GATO CABEZA
	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, dimVertices * sizeof(GLfloat), m_Vertices5,GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//GATO CABEZA
	glBindVertexArray(VAO[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, dimVertices * sizeof(GLfloat), m_Vertices6,GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//GATO OREJA 1
	glBindVertexArray(VAO[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, dimVertices * sizeof(GLfloat), m_Vertices7,GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//GATO OREJA 2
	glBindVertexArray(VAO[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
	glBufferData(GL_ARRAY_BUFFER, dimVertices * sizeof(GLfloat), m_Vertices8,GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//GATO COLA
	glBindVertexArray(VAO[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
	glBufferData(GL_ARRAY_BUFFER, dimVertices * sizeof(GLfloat), m_Vertices9,GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//CAPA_INVISIBLE

	glBindVertexArray(VAO[9]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
	glBufferData(GL_ARRAY_BUFFER, dimVertices * sizeof(GLfloat), m_Vertices10, GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//PIEDRA_RESURRECCION

	glBindVertexArray(VAO[10]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[10]);
	glBufferData(GL_ARRAY_BUFFER, dimVertices * sizeof(GLfloat), m_Vertices11, GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//VARITA_ZAUCO

	glBindVertexArray(VAO[11]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[11]);
	glBufferData(GL_ARRAY_BUFFER, dimVertices * sizeof(GLfloat), m_Vertices12, GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//LINEAS
	glBindVertexArray(VAO[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[12]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices13), m_Vertices13,GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}


void display(double currentTime) {
	glClearColor(0.196078f, 0.6f, 0.9f, 0.0f);
	glClear( GL_COLOR_BUFFER_BIT);


	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_LINE_LOOP, 0, numberOfVertices);

	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_LINE_LOOP, 0, numberOfVertices);

	glBindVertexArray(VAO[2]);
	glDrawArrays(GL_LINE_LOOP, 0, numberOfVertices);

	glBindVertexArray(VAO[3]);
	glDrawArrays(GL_LINE_STRIP, 2, (numberOfVertices/2)-3);

	glBindVertexArray(VAO[4]);
	glDrawArrays(GL_LINE_LOOP, 0, numberOfVertices);

	glBindVertexArray(VAO[5]);
	glDrawArrays(GL_LINE_LOOP, 0, numberOfVertices);

	glBindVertexArray(VAO[6]);
	glDrawArrays(GL_LINE_LOOP, 0, numberOfVertices);

	glBindVertexArray(VAO[7]);
	glDrawArrays(GL_LINE_LOOP, 0, numberOfVertices);

	glBindVertexArray(VAO[8]);
	glDrawArrays(GL_LINE_STRIP, 0, numberOfVertices*4/7);

	glBindVertexArray(VAO[9]);
	glDrawArrays(GL_LINE_LOOP, 0, numberOfVertices);

	glBindVertexArray(VAO[10]);
	glDrawArrays(GL_LINE_LOOP, 0,3);

	glBindVertexArray(VAO[11]);
	glDrawArrays(GL_LINES, 0,2);

	glBindVertexArray(VAO[12]);
	glDrawArrays(GL_LINES, 0, 4);
}

int main( void ) {
	// Initialise GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(800, 800, "Tarea03: Primitivas", NULL, NULL);

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	init();

	// Check if the window was closed
	while (!glfwWindowShouldClose(window)) {
		display(glfwGetTime());
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}





