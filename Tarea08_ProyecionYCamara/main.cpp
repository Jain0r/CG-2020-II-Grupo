#define GLEW_STATIC
#include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
 void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const float toRadians = 3.14159265f / 180.0f;
const float Grado = 180.0f / 3.14159265f;
bool activated = true;
bool vamos = false;
const float g = 9.8;
bool pv = true;

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// camera

Camera camera(glm::vec3(0.115745f, 3.93177f, 2.56463f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Test

float gravity = 9.81f;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tarea08_ProyecionYCamara", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	

	glfwSetScrollCallback(window, scroll_callback);
	glfwSetWindowPos(window, SCR_WIDTH/32, SCR_HEIGHT/25);

	


	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	 bool v = false;
	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);


	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	Shader tankShader("vertShader.glsl", "fragShader.glsl");
	Shader wallShader("vertShader.glsl", "fragShader.glsl");
	Shader cubeShader("lampVertShader.glsl", "lampFragShader.glsl");
	Shader canonShader("lampVertShader.glsl", "lampFragShader.glsl");
	Shader bulletShader("vertShader.glsl", "fragShader.glsl");

	Model tank("C:\\Users\\Mosho\\source\\repos\\Tarea08_ProyecionYCamara\\tank.obj");
	Model cube("C:\\Users\\Mosho\\source\\repos\\Tarea08_ProyecionYCamara\\cube.obj");
	Model target("C:\\Users\\Mosho\\source\\repos\\Tarea08_ProyecionYCamara\\Muro.obj");
	Model bullet("C:\\Users\\Mosho\\source\\repos\\Tarea08_ProyecionYCamara\\bala.obj");
	Model canon("C:\\Users\\Mosho\\source\\repos\\Tarea08_ProyecionYCamara\\canon_tank1.obj");

	float time = 0.5f; // two seconds

			// common part, do this only once
	
	float previous = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		
		if (v)
		{
			glfwSetCursorPosCallback(window, mouse_callback);
		}
		else
		{
			glfwSetCursorPosCallback(window, NULL);
		}
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		

		processInput(window);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		/*--[FRAME IMGUI]-----------*/
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH * 0.20f, SCR_HEIGHT * 0.15f));
		ImGui::SetNextWindowPos(ImVec2(15, 15));

		/*--[CODIGO IMGUI]-----------*/

		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameBorderSize = 1.0f;
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_MenuBar;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		ImVec2 buttonSize = ImVec2(80.0f, 20.f);


		ImGui::Begin("Camera Controls");
		ImGui::Spacing();
		ImGui::Indent(88.0f);
		if (ImGui::Button("Move UP", buttonSize)) {
			camera.ProcessKeyboard(FORWARD, deltaTime);
		}
		ImGui::Unindent(88.0f);
		ImGui::Spacing();

		if (ImGui::Button("Move LEFT", buttonSize)) {
			camera.ProcessKeyboard(LEFT, deltaTime);
		}
		ImGui::SameLine();
		if (ImGui::Button("Move DOWN", buttonSize)) {
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		}
		ImGui::SameLine();
		if (ImGui::Button("Move RIGHT", buttonSize)) {
			camera.ProcessKeyboard(RIGHT, deltaTime);
		}
		ImGui::End();
		static float angleX = 0.0f;
		static float MovAngleY = 1.0f*toRadians;
		static float angleY = 0.0f;
		static float angleZ = 0.0f;
		static float velocity = 16.0f;
		ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH * 0.20f, SCR_HEIGHT * 0.15f));
		ImGui::SetNextWindowPos(ImVec2(15, 200));
		ImGui::Begin("Shoot Controls");
		ImGui::SliderAngle("Der-Izq", &angleY, -20.0, 20.0);
		ImGui::SliderAngle("Arriba-Abajo", &angleX, 0.0, 20.0);
		ImGui::SliderFloat("Fuerza", &velocity, 1.0, 30.0);
		ImGui::End();

		ImGui::Begin("Control Del Aire");
		ImGui::SliderAngle("Viento", &MovAngleY, -20.0, 20.0);
		ImGui::End();

		ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH * 0.20f, SCR_HEIGHT * 0.15f));
		ImGui::SetNextWindowPos(ImVec2(15, 400));
		static int numberShots = 0;
		ImGui::Begin("Panel de Usuario");
		ImGui::TextWrapped("Numero de Disparos: ");
		ImGui::SameLine();
		string strShots = std::to_string(numberShots);
		const char* nShots = strShots.c_str();
		static int punt = 0;
		
		if (ImGui::Button("Disparo", buttonSize)) {
			glfwSetTime(0);
			activated = false;
			pv = true;
			
			}
		float now = glfwGetTime();
		float delta = now - previous;
		previous = now;
		time -= delta;

		if (activated== false ) {

			

			if (round(time) == 0)
			{
				if (velocity == 28.5) {
					if (angleX * Grado >= 14 && angleX * Grado <= 18) {
						if (angleY * Grado >= 7 && angleY * Grado <= 10) {
							while (pv) {
								punt = punt + 20;
								pv = false;
							}
							
						}
						if (angleY * Grado >= 3 && angleY * Grado <= 6 ) {
							while (pv) {
								punt = punt + 25;
								pv = false;
								
							}
							
						}
						if (angleY * Grado >= -4 && angleY * Grado <= -1) {
							while (pv) {
								punt = punt + 30;
								pv = false;
							}
							
						}

					}
					if (angleX * Grado >= 9 && angleX * Grado <= 12) {
						if (angleY * Grado >= 7 && angleY * Grado <= 10) {
							while (pv) {
								punt = punt + 35;
								pv = false;
							}
							
						}
						if (angleY * Grado >= 3 && angleY * Grado <= 6) {
							while (pv) {
								punt = punt + 40;
								pv = false;
							}
							
						}
						if (angleY * Grado >= -4 && angleY * Grado <= -1) {
							while (pv) {
								punt = punt + 45;
								pv = false;
							}
							
						}

					}
					if (angleX * Grado >= 6 && angleX * Grado <= 9) {
						if (angleY * Grado >= 7 && angleY * Grado <= 10) {
							while (pv) {
								punt = punt + 50;
								pv = false;
							}
							
						}
						if (angleY * Grado >= 3 && angleY * Grado <= 6) {
							while (pv) {
								punt = punt + 55;
								pv = false;
							}
							
						}
						if (angleY * Grado >= -4 && angleY * Grado <= -1) {
							while (pv) {
								punt = punt + 60;
								pv = false;
							}
							
						}

					}
					if (angleX * Grado >= 7 && angleX * Grado <= 10) {
						if (angleY * Grado >= -11 && angleY * Grado <= -6) {
							while (pv) {
								punt = punt + 80;
								pv = false;
							}
							
						}
					}
					if (angleX * Grado >= 11 && angleX * Grado <= 12) {
						if (angleY * Grado >= -10 && angleY * Grado <= -7) {
							while (pv) {
								punt = punt + 70;
								pv = false;
							}
							
						}
					}

				}




			}
			
		}

		
		
		ImGui::End();

		
		ImGui::Begin("Puntuacion");

			// ... rest of rendering goes here        
			string Punto = std::to_string(punt);



			const char* Puntaje = Punto.c_str();
			ImGui::Text(Puntaje);
		



		ImGui::End();

		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
			v = true;
		}
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
			v = false;
		}
		// Drawing Tank
		tankShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		tankShader.setMat4("projection", projection);
		tankShader.setMat4("view", view);
		tankShader.setMat4("model", model);
		tank.Draw(tankShader);

		//Drawing Targets
		wallShader.use();
		glm::mat4 modelTarget = glm::mat4(1.0f);
		modelTarget = glm::rotate(modelTarget, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelTarget = glm::translate(modelTarget, glm::vec3(30.0f, -1.0f, 0.0f));
		modelTarget = glm::scale(modelTarget, glm::vec3(0.1f, 0.1f, 0.1f));
		wallShader.setMat4("projection", projection);
		wallShader.setMat4("view", view);
		wallShader.setMat4("model", modelTarget);
		target.Draw(wallShader);


		//Drawing Bullet
		bulletShader.use();
		glm::mat4 modelBullet = glm::mat4(1.0f);

		modelBullet = glm::rotate(modelBullet, angleX, glm::vec3(1.0f, 0.0f, 0.0f));
		modelBullet = glm::rotate(modelBullet, angleY, glm::vec3(0.0f, 1.0f, 0.0f));

		
	   
		
		bulletShader.setMat4("projection", projection);
		bulletShader.setMat4("view", view);
		
		
		// Shot Function
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			glfwSetTime(0);
			activated = false;
		}
		// Print Camera
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			std::cout << "X: " << camera.Position.x << std::endl;
			std::cout << "Y: " << camera.Position.y << std::endl;
			std::cout << "Z: " << camera.Position.z << std::endl;
		}
		

		if (!activated) {
	
			static float parabolicoY = 0;
			static float U = -currentFrame * velocity;
			parabolicoY = U * sin(angleX*toRadians) * currentFrame - g * currentFrame * currentFrame/2.;
			static float ParabolicoX = 0;
			ParabolicoX =  MovAngleY * currentFrame*currentFrame;
			
		  
				modelBullet = glm::translate(modelBullet, glm::vec3(0.0f, 0.0f, -currentFrame * velocity*1.5));
				modelBullet = glm::translate(modelBullet, glm::vec3(ParabolicoX * 3, 0.0f, 0.0f));
				modelBullet = glm::translate(modelBullet, glm::vec3(0.0f, parabolicoY, 0.0f));
				
				modelBullet = glm::rotate(modelBullet, angleX, glm::vec3(1.0f, parabolicoY*1.5, 0.0f));
				

 
			

		}
		bulletShader.setMat4("model", modelBullet);
		bullet.Draw(bulletShader);
		
		// Drawing Canon
		tankShader.use();
		glm::mat4 modelCanon = glm::mat4(1.0f);
		modelCanon = glm::rotate(modelCanon, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//Handled
		modelCanon = glm::rotate(modelCanon, 0.9f * angleX , glm::vec3(1.0f, 0.0f, 0.0f));
		modelCanon = glm::rotate(modelCanon, 0.9f * angleY , glm::vec3(0.0f, 1.0f, 0.0f));
		modelCanon = glm::rotate(modelCanon, 0.9f * angleZ , glm::vec3(0.0f, 0.0f, 1.0f));


		modelCanon = glm::scale(modelCanon, glm::vec3(1.0f, 1.0f, 1.0f));
		tankShader.setMat4("projection", projection);
		tankShader.setMat4("view", view);
		tankShader.setMat4("model", modelCanon);
		canon.Draw(tankShader);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime*2.5);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime*2.5);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime*2.5);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime*2.5);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

 void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
