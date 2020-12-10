//============================================================================
// Name        : Display Animation using Shaders
// Professor   : Herminio Paucar
// Version     :
// Description : Display my first Animation
//============================================================================

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW

#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>

#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"
#include "Utils.h"
#define WIDTH 300
#define HEIGHT 200

int dimVertices;

int numberOfVertices;


float x = 0.0f;
float y = 0.0f;
float color_imagen[3]={0,0,0};
GLuint renderingProgram;
GLuint m_VBO;
GLuint m_VAO;



using namespace std;

void init (GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");

	// Cria um ID na GPU para um array de  buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	/*--[INICIALIZACION IMGUI]-----------*/
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	/*--[INICIALIZACION IMGUI]-----------*/
	ImGui::StyleColorsDark();

	GLfloat x = 0;
		GLfloat y = 0;
		GLfloat z = 0;
		GLfloat radius = 0.3;
		GLint numberOfSides = 50; // 50
		numberOfVertices = numberOfSides+2; // points + one center point.
		GLfloat twicePi = 2.0f * M_PI;

		GLfloat verticesX[numberOfVertices];
		GLfloat verticesY[numberOfVertices];
		GLfloat verticesZ[numberOfVertices];

		verticesX[0] = x;
		verticesY[0] = y;
		verticesZ[0] = z;
	    //vertic
		for (int i = 1; i < numberOfVertices; i++) {
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

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(
				GL_ARRAY_BUFFER,
				dimVertices * sizeof(GLfloat),
				m_Vertices,
				GL_STATIC_DRAW
			);


		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*) 0
		);

}

void display(GLFWwindow* window, double currentTime) {
	/*--[FRAME IMGUI]-----------*/
	    ImGui_ImplOpenGL3_NewFrame();
	    ImGui_ImplGlfw_NewFrame();
	    ImGui::NewFrame();

	    ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
	    ImGui::SetNextWindowPos(ImVec2(0,0));
	    /*--[FRAME IMGUI]-----------*/

	   // static ImVec4 color_fondo = ImVec4(0.5,0.7,0.9,1.0);
	    static float color_fondo[3]={0,0,0};
	    static float scale = 1;
	    static float color_figura[3]={1,0,0};
	    static int fondo = 0;
	    static int figura = 0;
	    GLuint uniformColor =glGetUniformLocation(renderingProgram, "u_color");
	    GLuint uniformModel = glGetUniformLocation(renderingProgram, "u_model");
	    //static float tras_x = 0, tras_y=0;
	    //static float rota_x = 0, rota_y = 0;
	    /*--[CODIGO IMGUI]-----------*/
	    ImGuiStyle& style = ImGui::GetStyle();
	    style.FrameBorderSize = 1.0f;
	    ImGui::Begin("Tarea05_IMGUI");
	    ImGui::TextColored(ImVec4(0.5, 0.9, 0.5, 1), "Menu de opciones: ");
	    if(ImGui::Button("arriba", ImVec2(190,.0)))
	    {
	    	y += 0.1f;
	    }
	    if(ImGui::Button("izquierda"))
	   	{
	   	    x += -0.1f;
	   	}
	    ImGui::SameLine();
	    if(ImGui::Button("abajo"))
	   	{
	   	   	y += -0.1f;
	   	}
	    ImGui::SameLine();
	    if(ImGui::Button("derecha"))
	 	{
	  	   	x += 0.1f;
	    }
	    ImGui::Indent( 16.0f );

	    if(ImGui::Button("Editar Color de Fondo"))
	    {
	    	fondo++;
	    }
	    if(fondo){
	    	ImGui::ColorPicker3("Editar color de fondo",color_fondo);
	    }
	    if(fondo==2){
	     	fondo=0;
	    }
	    if(ImGui::Button("Editar Color de la figura"))
	    {
	    	figura++;
	    }
	    if(figura){
	    	ImGui::ColorPicker3("Editar color de la figura",color_figura);
	    }
	    if(figura==2){
	    	figura=0;
	    }
	    ImGui::Unindent( .0 );
		ImGui::Text("Escalamiento");
		ImGui::SliderFloat("scala", &scale, 0, 4);
	    ImGui::End();
	    /*--[CODIGO IMGUI]-----------*/


	glUniform3f(uniformColor, color_figura[0], color_figura[1], color_figura[2]);
    glUseProgram(renderingProgram);
	glm::vec2 newPos(x, y);
	glm::mat4 trans = translate(glm::mat4(1), glm::vec3(x, y, 0));
	trans = glm::scale(trans, glm::vec3(scale, scale, 1));


    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(color_fondo[0], color_fondo[1],color_fondo[2], 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glm::mat4 model(1.0f);

    model = glm::scale(model, glm::vec3(scale,scale,1.0f));

    model = glm::translate(model,glm::vec3(x,y,0));


    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

    /*--[RENDER IMGUI]-----------*/
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    /*--[RENDER IMGUI]-----------*/
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(800, 800, "Tarea05_IMGUI", NULL, NULL);
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
