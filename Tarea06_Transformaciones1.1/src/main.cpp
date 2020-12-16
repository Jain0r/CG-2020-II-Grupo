//============================================================================
// Name        :Tarea: Tranformaciones
// Professor   : Herminio Paucar

//============================================================================

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include "Utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
int numberOfVertices;
int numberOfVertices2;
const float toRadians = 3.14159265f / 180.0f;

GLuint renderingProgram;
int dimVertices;
int dimVertices2;
GLfloat* m_Vertices[20];
GLuint n_Vertices[20];
GLuint m_VBO[20];
GLuint m_VAO[20];

float curAngle = 0.01f;
float curAngle2 = 0.0f;
float curAngle3 = 0.0f;
int pendulo = 1;

void init (GLFWwindow* window) {

	// Utils
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");

	// Cria um ID na GPU para um array de  buffers
	glGenVertexArrays(1, &m_VAO[0]);
	glBindVertexArray(m_VAO[0]);

	//base del cuerpo

	m_Vertices[0] = new GLfloat[24] {
		-0.15f, -0.45f, 0.0f, 0.690f, 0.572f, 0.231f,//base
		-0.15f, 0.12f, 0.0f,  0.690f, 0.572f, 0.231f,
		0.15f, 0.12f, 0.0f, 0.690f, 0.572f, 0.231f,
		0.15f, -0.45f , 0.0f, 0.84f,0.75f,0.49f

	};n_Vertices[0] = 24;
	glGenBuffers(1, &m_VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER,n_Vertices[0] * sizeof(GLfloat),m_Vertices[0],GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(1);		// Habilita este atributo

	glBindVertexArray(0);

	//fondo del reloj
	glGenVertexArrays(1, &m_VAO[1]);
	glBindVertexArray(m_VAO[1]);

	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat z = 0;
	GLfloat radius = 0.11;
	GLint numberOfSides = 50; // 50
	numberOfVertices = numberOfSides+2; // points + one center point.
	GLfloat twicePi = 2.0f * M_PI;

	GLfloat verticesX[numberOfVertices];
	GLfloat verticesY[numberOfVertices];
	GLfloat verticesZ[numberOfVertices];
	GLfloat verticesR[numberOfVertices];
	GLfloat verticesG[numberOfVertices];
	GLfloat verticesB[numberOfVertices];

	for (int i = 0; i < numberOfVertices; i++) {
		verticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		verticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		verticesZ[i] = z;
		verticesR[i] = 0.984;
		verticesG[i] = 0.949;
		verticesB[i] = 0.815;

		if(i<10){
			verticesR[i]=0.972;
			verticesG[i] = 0.894;
			verticesB[i] = 0.588;
		}
	}

	dimVertices = (numberOfVertices) * 3;


	GLfloat m_Vertices2[dimVertices*2];
	for (int i = 0; i < numberOfVertices; i++) {
		m_Vertices2[i * 6] = verticesX[i];
		m_Vertices2[i * 6 + 1] = verticesY[i];
		m_Vertices2[i * 6 + 2] = verticesZ[i];
		m_Vertices2[i*6+3]= verticesR[i];
		m_Vertices2[i*6+4]= verticesG[i];
		m_Vertices2[i*6+5]= verticesB[i];

	};

		glGenBuffers(1, &m_VBO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);

		glBufferData(GL_ARRAY_BUFFER,dimVertices*2 * sizeof(GLfloat),m_Vertices2,GL_STATIC_DRAW);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);		// Habilita este atributo
		glBindVertexArray(0);

	//crear los puntos del reloj
		glGenVertexArrays(1, &m_VAO[2]);
		glBindVertexArray(m_VAO[2]);

		x = 0;
		y = 0;
		z = 0;
		radius = 0.09;
		GLfloat numberOfSides2 = 12; // 50
		numberOfVertices2 = numberOfSides2+2; // points + one center point.
		twicePi = 2.0f * M_PI;


		for (int i = 0; i < numberOfVertices2; i++) {
			verticesX[i] = x + (radius * cos(i * twicePi / numberOfSides2));
			verticesY[i] = y + (radius * sin(i * twicePi / numberOfSides2));
			verticesZ[i] = z;
			verticesR[i] = 0;
			verticesG[i] = 0;
			verticesB[i] = 0;
		}


		dimVertices2 = (numberOfVertices2) * 3;
		GLfloat m_Vertices3[dimVertices2*2];
		m_Vertices3[0] = -0.15;
		m_Vertices3[0] = 0.19;
		m_Vertices3[0] = 0;
		verticesR[0] = 0;
		verticesG[0] = 0;
		verticesB[0] = 0;

		for (int i = 0; i < numberOfVertices2; i++) {
			m_Vertices3[i * 6] = verticesX[i];
			m_Vertices3[i * 6 + 1] = verticesY[i];
			m_Vertices3[i * 6 + 2] = verticesZ[i];
			m_Vertices3[i*6+3]= verticesR[i];
			m_Vertices3[i*6+4]= verticesG[i];
			m_Vertices3[i*6+5]= verticesB[i];

		};

			glGenBuffers(1, &m_VBO[2]);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);

			glBufferData(GL_ARRAY_BUFFER,dimVertices2*2 * sizeof(GLfloat),m_Vertices3,GL_STATIC_DRAW);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);		// Habilita este atributo
			glBindVertexArray(0);

	//Minutero
			glGenVertexArrays(1, &m_VAO[3]);
			glBindVertexArray(m_VAO[3]);

			m_Vertices[3] = new GLfloat[42] {
				0.003f,0.05f, 0.0f, 0.0f, 0.0f, 0.0f,
				-0.003f, 0.05f, 0.0f, 0.0f, 0.0f,0.0f,
				-0.003f, 0.0f ,0.0f, 0.0f, 0.0f,0.0f,
				0.003f,0.0f, 0.0f, 0.0f, 0.0f,0.0f, //cuerpo
				0.007f,0.05f,0.0f, 0.0f, 0.0f,0.0f,
				0.0f,0.08f, 0.0f, 0.0f, 0.0f,0.0f,
				-0.007f,0.05f, 0.0f, 0.0f, 0.0f,0.0f

			};n_Vertices[3] = 42;
			glGenBuffers(1, &m_VBO[3]);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO[3]);
			glBufferData(GL_ARRAY_BUFFER,n_Vertices[3] * sizeof(GLfloat),m_Vertices[3],GL_STATIC_DRAW);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));

			glEnableVertexAttribArray(1);		// Habilita este atributo

			glBindVertexArray(0);

		//Horario
			glGenVertexArrays(1, &m_VAO[4]);
			glBindVertexArray(m_VAO[4]);

			m_Vertices[4] = new GLfloat[42] {
				0.003f,0.03f, 0.0f, 0.0f, 0.0f, 0.0f,
				-0.003f, 0.03f, 0.0f, 0.0f, 0.0f,0.0f,
				-0.003f, 0.0f ,0.0f, 0.0f, 0.0f,0.0f,
				0.003f,0.0f, 0.0f, 0.0f, 0.0f,0.0f, //cuerpo
				0.007f,0.03f,0.0f, 0.0f, 0.0f,0.0f,
				0.0f,0.06f, 0.0f, 0.0f, 0.0f,0.0f,
				-0.007f,0.03f, 0.0f, 0.0f, 0.0f,0.0f

			};n_Vertices[4] = 42;
			glGenBuffers(1, &m_VBO[4]);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO[4]);
			glBufferData(GL_ARRAY_BUFFER,n_Vertices[4] * sizeof(GLfloat),m_Vertices[4],GL_STATIC_DRAW);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));

			glEnableVertexAttribArray(1);		// Habilita este atributo

			glBindVertexArray(0);

			//cuerpo Pendulo
			glGenVertexArrays(1, &m_VAO[5]);
			glBindVertexArray(m_VAO[5]);

			m_Vertices[5] = new GLfloat[24] {
				0.01f,-0.11f, 0.0f, 0.0f, 0.0f, 0.0f,
				-0.01f, -0.11f, 0.0f, 0.0f, 0.0f,0.0f,
				-0.01f, -0.25f ,0.0f, 0.0f, 0.0f,0.0f,
				0.01f,-0.25f, 0.0f, 0.262f, 0.235f, 0.137f, //cuerpo


			};n_Vertices[5] = 24;
			glGenBuffers(1, &m_VBO[5]);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO[5]);
			glBufferData(GL_ARRAY_BUFFER,n_Vertices[5] * sizeof(GLfloat),m_Vertices[5],GL_STATIC_DRAW);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));

			glEnableVertexAttribArray(1);		// Habilita este atributo

			glBindVertexArray(0);


			//Circulo del Pendulo
			glGenVertexArrays(1, &m_VAO[6]);
			glBindVertexArray(m_VAO[6]);

			x = 0;
			y = -0.282;
			z = 0;
			radius = 0.035;
			twicePi = 2.0f * M_PI;


			for (int i = 0; i < numberOfVertices; i++) {
			verticesX[i] = x + (radius * cos(i * twicePi / numberOfSides2));
			verticesY[i] = y + (radius * sin(i * twicePi / numberOfSides2));
			verticesZ[i] = z;
			verticesR[i] = 0;
			verticesG[i] = 0;
			verticesB[i] = 0;
			if(i>8){
				verticesR[i] = 0.262;
				verticesG[i] = 0.235;
				verticesB[i] = 0.137;

			}
			if(i<3){
				verticesR[i] = 0.262;
				verticesG[i] = 0.235;
				verticesB[i] = 0.137;

			}
			}


			dimVertices = (numberOfVertices) * 3;


			for (int i = 0; i < numberOfVertices; i++) {
			m_Vertices3[i * 6] = verticesX[i];
			m_Vertices3[i * 6 + 1] = verticesY[i];
			m_Vertices3[i * 6 + 2] = verticesZ[i];
			m_Vertices3[i*6+3]= verticesR[i];
			m_Vertices3[i*6+4]= verticesG[i];
			m_Vertices3[i*6+5]= verticesB[i];

			};

			glGenBuffers(1, &m_VBO[6]);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO[6]);

			glBufferData(GL_ARRAY_BUFFER,dimVertices2*2 * sizeof(GLfloat),m_Vertices3,GL_STATIC_DRAW);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);		// Habilita este atributo
			glBindVertexArray(0);

			//Circulo que crece
			glGenVertexArrays(1, &m_VAO[19]);
			glBindVertexArray(m_VAO[19]);

			x = 0;
			y = 0.0;
			z = 0;
			radius = 0.05;
			twicePi = 2.0f * M_PI;


			for (int i = 0; i < numberOfVertices2; i++) {
			verticesX[i] = x + (radius * cos(i * twicePi / numberOfSides2));
			verticesY[i] = y + (radius * sin(i * twicePi / numberOfSides2));
			verticesZ[i] = z;
			verticesR[i] = 0.384;
			verticesG[i] = 0.239;
			verticesB[i] = 0.4;

			if(i>5){
							verticesR[i] = 0.223;
							verticesG[i] = 0.145;
							verticesB[i] = 0.235;
						}
			}


			dimVertices = (numberOfVertices2) * 3;


			for (int i = 0; i < numberOfVertices2; i++) {
			m_Vertices3[i * 6] = verticesX[i];
			m_Vertices3[i * 6 + 1] = verticesY[i];
			m_Vertices3[i * 6 + 2] = verticesZ[i];
			m_Vertices3[i*6+3]= verticesR[i];
			m_Vertices3[i*6+4]= verticesG[i];
			m_Vertices3[i*6+5]= verticesB[i];

			};

			glGenBuffers(1, &m_VBO[19]);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO[19]);

			glBufferData(GL_ARRAY_BUFFER,dimVertices2*2 * sizeof(GLfloat),m_Vertices3,GL_STATIC_DRAW);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);		// Habilita este atributo
			glBindVertexArray(0);


			//fondo del Reloj
			glGenVertexArrays(1, &m_VAO[7]);
			glBindVertexArray(m_VAO[7]);

			m_Vertices[7] = new GLfloat[24] {
				-0.17f, -0.47f, 0.0f,  0.341f, 0.286f, 0.117f,//base
				-0.17f, 0.14f, 0.0f, 0.341f, 0.286f, 0.117f,
				0.17f, 0.14f, 0.0f,  0.341f, 0.286f, 0.117f,
				0.17f, -0.47f , 0.0f, 0.84f,0.75f,0.49f


			};n_Vertices[7] = 24;
			glGenBuffers(1, &m_VBO[7]);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO[7]);
			glBufferData(GL_ARRAY_BUFFER,n_Vertices[7] * sizeof(GLfloat),m_Vertices[7],GL_STATIC_DRAW);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));

			glEnableVertexAttribArray(1);		// Habilita este atributo

			glBindVertexArray(0);

			//fondo del pendulo

			glGenVertexArrays(1, &m_VAO[8]);
			glBindVertexArray(m_VAO[8]);

			m_Vertices[8] = new GLfloat[24] {
				0.09f,-0.11f, 0.0f, 0.239f, 0.203f, 0.101f,
				-0.09f, -0.11f, 0.0f, 0.239f, 0.203f, 0.101f,
				-0.09f, -0.35f ,0.0f, 0.239f, 0.203f, 0.101f,
				0.09f,-0.35f, 0.0f, 0.133, 0.117, 0.066f //cuerpo



			};n_Vertices[8] = 24;
			glGenBuffers(1, &m_VBO[8]);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO[8]);
			glBufferData(GL_ARRAY_BUFFER,n_Vertices[8] * sizeof(GLfloat),m_Vertices[8],GL_STATIC_DRAW);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));

			glEnableVertexAttribArray(1);		// Habilita este atributo

			glBindVertexArray(0);

			//Marco del Reloj
			glGenVertexArrays(1, &m_VAO[9]);
			glBindVertexArray(m_VAO[9]);

			 x = 0;
			 y = 0;
			 z = 0;
			 radius = 0.118;

			for (int i = 0; i < numberOfVertices; i++) {
				verticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
				verticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
				verticesZ[i] = z;
				verticesR[i] = 0.478;
				verticesG[i] = 0.396;
				verticesB[i] =  0.082;

				if(i<20){
					verticesR[i]=0.533;
					verticesG[i] =0.443;
					verticesB[i] = 0.105;;
				}
			}

			dimVertices = (numberOfVertices) * 3;


			GLfloat m_Vertices5[dimVertices*2];
			for (int i = 0; i < numberOfVertices; i++) {
				m_Vertices5[i * 6] = verticesX[i];
				m_Vertices5[i * 6 + 1] = verticesY[i];
				m_Vertices5[i * 6 + 2] = verticesZ[i];
				m_Vertices5[i*6+3]= verticesR[i];
				m_Vertices5[i*6+4]= verticesG[i];
				m_Vertices5[i*6+5]= verticesB[i];

			};

				glGenBuffers(1, &m_VBO[9]);
				glBindBuffer(GL_ARRAY_BUFFER, m_VBO[9]);

				glBufferData(GL_ARRAY_BUFFER,dimVertices*2 * sizeof(GLfloat),m_Vertices5,GL_STATIC_DRAW);
				glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));
				glEnableVertexAttribArray(1);		// Habilita este atributo
				glBindVertexArray(0);


			//fondo de la cabeza

			glGenVertexArrays(1, &m_VAO[10]);
			glBindVertexArray(m_VAO[10]);

			m_Vertices[10] = new GLfloat[18] {
				-0.22f, 0.12f, 0.0f, 0.341f, 0.286f, 0.117f,
				0.22f, 0.12f, 0.0f, 0.403f, 0.349f, 0.133f,
				0.0f, 0.3f , 0.0f, 0.341f, 0.286f, 0.117f

			};n_Vertices[10] = 18;
			glGenBuffers(1, &m_VBO[10]);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO[10]);
			glBufferData(GL_ARRAY_BUFFER,n_Vertices[10] * sizeof(GLfloat),m_Vertices[10],GL_STATIC_DRAW);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));

			glEnableVertexAttribArray(1);		// Habilita este atributo

			glBindVertexArray(0);

			//la cabeza

			glGenVertexArrays(1, &m_VAO[11]);
			glBindVertexArray(m_VAO[11]);

			m_Vertices[11] = new GLfloat[18] {
				-0.17f, 0.14f, 0.0f,0.580f, 0.498f, 0.180f,
				0.17f, 0.14f, 0.0f, 0.84f,0.75f,0.49f,
				0.0f, 0.27f , 0.0f, 0.580f, 0.498f, 0.180f

			};n_Vertices[11] = 18;
			glGenBuffers(1, &m_VBO[11]);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO[11]);
			glBufferData(GL_ARRAY_BUFFER,n_Vertices[11] * sizeof(GLfloat),m_Vertices[11],GL_STATIC_DRAW);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));

			glEnableVertexAttribArray(1);		// Habilita este atributo

			glBindVertexArray(0);

			//fondo de la base

			glGenVertexArrays(1, &m_VAO[12]);
			glBindVertexArray(m_VAO[12]);

			m_Vertices[12] = new GLfloat[24] {
				-0.17f, -0.47f, 0.0f,  0.341f, 0.286f, 0.117f,//base
				-0.22f, -0.55f, 0.0f, 0.341f, 0.286f, 0.117f,
				0.23f, -0.55f, 0.0f,  0.341f, 0.286f, 0.117f,
				0.17f, -0.47f , 0.0f, 0.84f,0.75f,0.49f


			};n_Vertices[12] = 124;
			glGenBuffers(1, &m_VBO[12]);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO[12]);
			glBufferData(GL_ARRAY_BUFFER,n_Vertices[12] * sizeof(GLfloat),m_Vertices[12],GL_STATIC_DRAW);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));

			glEnableVertexAttribArray(1);		// Habilita este atributo

			glBindVertexArray(0);

			//fondo pendulov2

			glGenVertexArrays(1, &m_VAO[13]);
			glBindVertexArray(m_VAO[13]);

			m_Vertices[13] = new GLfloat[24] {
				0.093f,-0.11f, 0.0f,0.341f, 0.286f, 0.117f,
				-0.093f, -0.11f, 0.0f, 0.341f, 0.286f, 0.117f,
				-0.093f, -0.35f ,0.0f, 0.341f, 0.286f, 0.117f,
				0.093f,-0.35f, 0.0f, 0.84f,0.75f,0.49f //cuerpo



			};n_Vertices[13] = 24;
			glGenBuffers(1, &m_VBO[13]);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO[13]);
			glBufferData(GL_ARRAY_BUFFER,n_Vertices[13] * sizeof(GLfloat),m_Vertices[13],GL_STATIC_DRAW);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*) (3 * sizeof(GLfloat)));

			glEnableVertexAttribArray(1);		// Habilita este atributo

			glBindVertexArray(0);

}


void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.4, 0.364, 0.239, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	GLuint uniformModel = glGetUniformLocation(renderingProgram, "mv");

	GLuint rotateLoc = glGetUniformLocation(renderingProgram, "rotate");


	curAngle += 0.098;

	if (curAngle >= 360)
	{
		curAngle -= 360;
		curAngle2 += 30;
	}

	if(curAngle2 >=360)
	{
		curAngle2 -= 360;
	}
	if(pendulo){
		glfwSetTime(1);
		curAngle3+=glfwGetTime();
		if(curAngle3>=90/2){
			pendulo=0;
		}
	}else{
		glfwSetTime(1);
		curAngle3-=glfwGetTime();
		if(curAngle3<=-45/2){
			pendulo=1;
		}
	}




	// Matriz con elementos de valor 1
	glm::mat4 model(1.0f);
	glm::mat4 vMat =glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.25,0.0f));
	glm::mat4 mvMat;
	mvMat = model*vMat;
	//Giro Antihorario
	//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

	//Usando UniformMatrix
	//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//Usando ProgramUniform
	//glProgramUniformMatrix4fv(renderingProgram, uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//glm::mat4 model(1.0f);
	    //model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mvMat));

	// Use este VAO e suas configurações

	glProgramUniform1f(renderingProgram, rotateLoc,curAngle);


	 glBindVertexArray(m_VAO[19]);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 1, numberOfVertices,2);


    //Bordes
    glBindVertexArray(m_VAO[7]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    //fondo
    glBindVertexArray(m_VAO[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    //fondo del Pendulo
    glBindVertexArray(m_VAO[8]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    //Marco del reloj
    glBindVertexArray(m_VAO[9]);
    glDrawArrays(GL_TRIANGLE_FAN, 1, numberOfVertices);




    //fondo del reloj
    glBindVertexArray(m_VAO[1]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

    //fondo de la cabeza
    glBindVertexArray(m_VAO[10]);
    glDrawArrays(GL_TRIANGLE_FAN,0,3);

    //la cabeza
    glBindVertexArray(m_VAO[11]);
    glDrawArrays(GL_TRIANGLE_FAN,0,3);

    //fondo de la base
    glBindVertexArray(m_VAO[12]);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);

    //fondo del pendeluov2
    glBindVertexArray(m_VAO[13]);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);



    //puntos del reloj
    glBindVertexArray(m_VAO[2]);
    glPointSize(5.0f);
    glDrawArrays(GL_POINTS, 0, numberOfVertices2);


    model = glm::mat4(1.0f);
    model = glm::rotate(model, curAngle*-1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    mvMat=vMat*model;

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mvMat));
    //segundero
    glBindVertexArray(m_VAO[3]);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLES,4,7);

    model = glm::mat4(1.0f);
    model = glm::rotate(model, curAngle2*-1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    mvMat=vMat*model;

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mvMat));

    //minutero
    glBindVertexArray(m_VAO[4]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLES,4,7);

    model = glm::mat4(1.0f);
    model = glm::rotate(model, (curAngle3*1/4-4) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    mvMat=vMat*model;

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mvMat));

    glBindVertexArray(m_VAO[5]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(m_VAO[6]);
    glDrawArrays(GL_TRIANGLE_FAN, 1, numberOfVertices);

    //model =glm::mat4(1.0f);
    //model = glm::rotate(model, curAngle*-1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));


    //model = glm::translate(model,glm::vec3(curAngle/50,0.0f,1.0f));
    //mvMat=vMat*model;
    //glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mvMat));
    //model = glm::rotate(model,curAngle*-1 * toRadians,glm::vec3(0.0f,0.0f,1.0f));


    glBindVertexArray(0);
	glUseProgram(0);
}

int main(void) {
    if (!glfwInit()) {
    	exit(EXIT_FAILURE);
    }
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(800, 800, "Tarea: Transformaciones", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
    	exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
