#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/calib3d/calib3d_c.h>
#include <opencv2/opencv.hpp>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>


// Include GLFW
#include <GLFW/glfw3.h>

#include <SOIL2/SOIL2.h>
#include "Utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <math.h>

#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>


#include <stack>
#include <time.h>

using namespace std;
using namespace cv;

float curAngle = 0.0f;
const float toRadians = 3.14159265f / 180.0f;
std::stack<glm::mat4> mvStack;
std::stack<glm::mat4> mvStack2;

const float calibrationSquareDimesion = 0.01905f; //metros
const float arucoSquareDimension = 0.05f; //metros
const Size chessboardDimensions = Size(6, 9);
int width = 640, height = 480;
GLuint m_VBO;
GLuint m_VAO;
GLuint renderingProgram;
GLfloat* m_Vertices;
GLuint n_Vertices;



void crearArucoMarkers()
{
    Mat outputMarker;

    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

    for(int i = 0; i < 2; i++)
    {
        aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
        ostringstream convert;
        string imageName = "4x4Marker_";
        convert << imageName << i << ".jpg";
        imwrite(convert.str(), outputMarker);
    }
}
void CrearPosicionTableroConocido (Size boardSize , float LonBordeCuadrado, vector<Point3f>& esquinas)
{
    for(int i=0; i < boardSize.height; i++)
    {
        for(int j = 0 ; j < boardSize.width ; j++)
        {
        	esquinas.push_back(Point3f(j * LonBordeCuadrado, i * LonBordeCuadrado, 0.0f));
        }
    }
}

void ConseguirEsquinasTablero(vector<Mat> imagenes, vector<vector<Point2f>>& TodasEsquinasEncontradas, bool MostrarResultados = false)
{
    for (vector <Mat>::iterator iter = imagenes.begin(); iter != imagenes.end(); iter++)
    {
        vector < Point2f> pointBuf;
        bool encontrar = findChessboardCorners(*iter, Size(9, 6), pointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);

        if (encontrar)
        {
        	TodasEsquinasEncontradas.push_back(pointBuf);
        }

        if (MostrarResultados)
        {
            drawChessboardCorners(*iter, Size(9, 6), pointBuf, encontrar);
            imshow("buscando esquinas", *iter);
            waitKey(0);
        }
    }
}

void CalibracionCamara (vector <Mat> ImagenesCalibracion, Size TamanoTablero, float LongBordeCuadrado, Mat& camaraMatrix, Mat& distanciaCoeficiente)
{
	vector < vector <Point2f>> PuntosEspacioImagenTablero;
	ConseguirEsquinasTablero(ImagenesCalibracion, PuntosEspacioImagenTablero, false );

	vector < vector  <Point3f> > EspacioCordenadas(1) ;

	CrearPosicionTableroConocido( TamanoTablero, LongBordeCuadrado, EspacioCordenadas [0]);

	EspacioCordenadas.resize(PuntosEspacioImagenTablero.size(), EspacioCordenadas[0]);

	vector<Mat> rVectors, tVectors;
	distanciaCoeficiente = Mat::zeros(8,1 , CV_64F);

	 calibrateCamera(EspacioCordenadas,PuntosEspacioImagenTablero, TamanoTablero, camaraMatrix, distanciaCoeficiente, rVectors, tVectors );


}

bool GuardarCalibracion(String nombre ,Mat MatrizCamara, Mat distanciaCoe)
{
	ofstream outstream(nombre);
	if(outstream)
	{
		uint16_t rows = MatrizCamara.rows ;
		uint16_t columns = MatrizCamara.cols;

		for(int r = 0;r < rows; r++)
		{
			for(int c = 0; c< columns; c++)
			{
				double value = MatrizCamara.at<double>(r,c);
				outstream << value << endl;
			}
		}

		rows = distanciaCoe.rows;
		columns = distanciaCoe.cols;

		for(int r = 0;r < rows; r++)
		{
			for(int c = 0; c< columns; c++)
			{
				double value = MatrizCamara.at<double>(r,c);
				outstream << value << endl;
			}
		}

		outstream.close();
		return true;

	}

	return false;
}

bool CargarCamaraCalibracion(string nombre, Mat& cameraMatrix, Mat& distanciaCoe)
{
    ifstream inStream(nombre);
    if (inStream)
    {
        uint16_t rows;
        uint16_t columns;

        inStream >> rows;
        inStream >> columns;

        cameraMatrix = Mat(Size(columns, rows), CV_64F);

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < columns; c++)
            {
                double read = 0.0f;
                inStream >> read;
                cameraMatrix.at<double>(r, c) = read;
                cout << cameraMatrix.at<double>(r, c) << "\n";
            }
        }
        //Distancia de los coeficientes
        inStream >> rows;
        inStream >> columns;

        distanciaCoe = Mat::zeros(rows, columns, CV_64F);

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < columns; c++)
            {
                double read = 0.0f;
                inStream >> read;
                distanciaCoe.at<double>(r, c) = read;
                cout << distanciaCoe.at<double>(r, c) << "\n";
            }
        }
        inStream.close();
        return true;

    }

    return false;
}


int WebcamMonitoreo(const Mat& cameraMatrix, const Mat& distanciaCoe, float arucoDimensionCuadrado)
{
    Mat frame;

    vector<int> marcadorIds;
    vector<vector<Point2f>> marcadorEsquinas, candidatosRechazado;
    aruco::DetectorParameters parameters;

    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);


    VideoCapture vid(0);
    if (vid.isOpened() == false)  //  verificar si el objeto detecta
    {
        std::cout << "error: webcan no se conecto exitosamente/n"; // if not then print error message
        return(0);            // salir del programa
    }
    if (!vid.isOpened())
    {
    	return -1;
    }



    namedWindow("Webcam", WINDOW_AUTOSIZE);

    vector<Vec3d> rotationVectors, translationVectors;

    while (true)
    {
        if (!vid.read(frame))
            break;

        aruco::detectMarkers(frame, markerDictionary, marcadorEsquinas, marcadorIds);
        aruco::estimatePoseSingleMarkers(marcadorEsquinas, arucoDimensionCuadrado, cameraMatrix,distanciaCoe, rotationVectors, translationVectors);

        for (int i = 0; i < (int) marcadorIds.size(); i++)
        {
            aruco::drawAxis(frame, cameraMatrix, distanciaCoe, rotationVectors, translationVectors, 0.1f);
        }
        imshow("Webcam", frame);
        if (waitKey(30) >= 0) break;
    }
    return 1;
}

void ProcesoCalibracionCamara(Mat& camaraMatrix, Mat& DistanciaCoeficiente)
{
	Mat Frame;
		Mat DibujarFrame;



		vector<Mat> GuardarImagen;

		vector<vector<Point2f>> EsquinasMarcador, CandidatosRecha;

		VideoCapture vid(0);

		if(!vid.isOpened())
		{
			return ;
		}

		int framesPorSeg = 20;

		namedWindow("Webcam", WINDOW_AUTOSIZE);

		while(true)
		{
			if(!vid.read(Frame))
				break;
			vector<Vec2f> PuntosEncontrados;
			bool Encontrar = false;

			Encontrar = findChessboardCorners(Frame, chessboardDimensions, PuntosEncontrados, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE );

			Frame.copyTo(DibujarFrame);
			drawChessboardCorners(DibujarFrame,chessboardDimensions, PuntosEncontrados, Encontrar);
			if(Encontrar)
				imshow("Webcam", DibujarFrame);
			else
				imshow("Webcam", Frame);
			char character = waitKey(1000 / framesPorSeg) ;

			switch( character)
			{
			case ' ':
				//guardar imagenes
				if(Encontrar)
				{
					Mat temp;
					Frame.copyTo(temp);
					GuardarImagen.push_back(temp);

				}
				break;
			case 13 :
				//comenzar calibracion
				if(GuardarImagen.size()>15)
				{
					CalibracionCamara(GuardarImagen, chessboardDimensions, calibrationSquareDimesion, camaraMatrix, DistanciaCoeficiente);
					GuardarCalibracion("CalibraciondeCamara" , camaraMatrix, DistanciaCoeficiente);
				}

				break;
			case 27 :
				//salida
				return ;
				break;



			}
		}

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//When the user presses the escape key, we set the window should close property to true, and close the application.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}




int main()
{
	// OpenCV
	cv::Mat rot_mat;
	cv::Mat rotationVectors, translationVectors;

	// OpenCV
	cv::Mat rot_mat2;
	cv::Mat rotationVectors2, translationVectors2;


	// GLFW ====================================================================
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Sistema Solar", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// GLEW ====================================================================
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// GLAD ====================================================================
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// Viewport dimensions
	glViewport(0, 0, 800, 600);

	// Shaders
	Shader Sol("vertShader.glsl", "fragShader.glsl");
	Shader Luna("vertShader.glsl", "fragShader.glsl");
	Shader Tierra("vertShader.glsl", "fragShader.glsl");
	Shader background("bgVertShader.glsl", "bgFragShader.glsl");
	Shader lampShader("lampVertShader.glsl", "lampFragShader.glsl");
	GLuint bgProgram = Utils::createShaderProgram("bgVertShader.glsl", "bgFragShader.glsl");

	// Models
	Model mSol("C:\\Users\\Mosho\\source\\repos\\Proyecto\\sol.obj");
	Model mLuna("C:\\Users\\Mosho\\source\\repos\\Proyecto\\Luna.obj");
	Model mTierra("C:\\Users\\Mosho\\source\\repos\\Proyecto\\Tierra.obj");
	Model lampModel("C:\\Users\\Mosho\\source\\repos\\Proyecto\\cube.obj");

	// Camera Matrix & Distortion Parameters
	cv::Mat intrinsic_matrix = cv::Mat::eye(3, 3, CV_64F);
	cv::Mat distortionParameters;

	CargarCamaraCalibracion("CalibraciondeCamara", intrinsic_matrix, distortionParameters);

	// Video Variables
	cv::Mat frame;
	std::vector<int> markerIds;
	std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
	cv::aruco::DetectorParameters parameters;
	cv::Ptr<cv::aruco::Dictionary> markerDictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
	cv::Ptr<cv::aruco::GridBoard> board = cv::aruco::GridBoard::create(4, 4, 0.04, 0.01, markerDictionary);

	// Video Variables 2
	cv::Mat frame2;
	std::vector<int> markerIds2;
	std::vector<std::vector<cv::Point2f>> markerCorners2, rejectedCandidates2;
	cv::aruco::DetectorParameters parameters2;
	cv::Ptr<cv::aruco::Dictionary> markerDictionary2 = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_5X5_50);
	cv::Ptr<cv::aruco::GridBoard> board2 = cv::aruco::GridBoard::create(4, 4, 0.04, 0.01, markerDictionary2);

	cv::VideoCapture vid(1);

	if (!vid.isOpened()) {
		return -1;
	}


	// ----------------------------------------------------------------------------------------------
	// Scaling models
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mLuna1 = glm::mat4(1.0f);
	glm::mat4 mTierra1 = glm::mat4(1.0f);

	// model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	mLuna1 = glm::scale(mLuna1, glm::vec3(0.05f, 0.05f, 0.05f));
	mTierra1 = glm::scale(mTierra1, glm::vec3(0.05f, 0.05f, 0.05f));

	// Initial default value (invisible)
	glm::mat4 modelview = glm::mat4(1.0f);
	glm::mat4 mvLuna = glm::mat4(1.0f);
	glm::mat4 mvTIerra = glm::mat4(1.0f);

	modelview = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
	mvLuna = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
	mvTIerra = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));

	// Defining the projection matrix manually
	float near = 0.2f;
	float far = 400.0f;
	float fx = 612.9770191846128f;
	float fy = 612.9770191846128f;
	float cx = 319.5;
	float cy = 239.5;

	// OpenCV -> OpenGL perspective projection matrix generation
	glm::mat4 projection_perspective = { fx / cx,                  0,                   0,                                   0,
											0,                  fy / cy,                0,                                   0,
											0,                     0,     -(far + near) / (far - near),     -(2 * far * near) / (far - near),
											0,                     0,                  -1,                                   0 };


	projection_perspective = glm::transpose(projection_perspective);

	// ----------------------------------------------------------------------------------------------
	// Background object definitions
	GLfloat vertices_bg[] =
	{
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
	};
	// -----------------------------------------------------------------------------------------------
	// Background object
	GLuint VBO_bg, VAO_bg;
	glGenVertexArrays(1, &VAO_bg);
	glGenBuffers(1, &VBO_bg);
	glBindVertexArray(VAO_bg);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bg);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_bg), vertices_bg, GL_STATIC_DRAW);
	// Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Texture Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); // Unbind VAO_bg

	 // -----------------------------------------------------------------------------------------------
	//  Webcam texture
	vid >> frame;
	width = frame.size().width;
	height = frame.size().height;

	GLuint texture_bg;
	glGenTextures(1, &texture_bg);
	glBindTexture(GL_TEXTURE_2D, texture_bg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// --------------------------------------------------------------------------------------------------
	// Transformation presets for the background object (model, view and orthographic projection)
	glm::mat4 modelview_bg = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, -399.8, 0, 0, 0, 1 };
	modelview_bg = glm::transpose(modelview_bg);
	modelview_bg = glm::scale(modelview_bg, glm::vec3(798 * cx / fx, 798 * cy / fy, 0));

	// --------------------------------------------------------------------------------------------------
	// Ambient light
	float ambient_strength = 0.2f;
	glm::vec3 light_color = glm::vec3(255.0f / 255.0f, 219.0f / 255.0f, 58.0f / 255.9f);
	glm::vec3 ambient_light = ambient_strength * light_color;

	// --------------------------------------------------------------------------------------------------
	// Display

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = glfwGetTime();

		glm::mat4 modelview = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 10, 0, 0, 0, 1 };
		glm::mat4 modelview2 = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 10, 0, 0, 0, 1 };
		glm::mat4 mvLuna = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 10, 0, 0, 0, 1 };
		glm::mat4 mvTIerra = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 10, 0, 0, 0, 1 };

		vid >> frame;
		vid >> frame2;
		cv::aruco::detectMarkers(frame, markerDictionary, markerCorners, markerIds);
		cv::aruco::detectMarkers(frame2, markerDictionary2, markerCorners2, markerIds2);
		//  Detecting markers
		if (markerIds.size() > 0) {
			// Try to find markers
			int valid = estimatePoseBoard(markerCorners, markerIds, board, intrinsic_matrix, distortionParameters, rotationVectors, translationVectors);

			// If OpenCV finds it, then draw the model on top of it.
			if (valid > 0) {
				cv::Rodrigues(rotationVectors, rot_mat);

				modelview = { rot_mat.at<double>(0,0),  rot_mat.at<double>(0,1),  rot_mat.at<double>(0,2),  translationVectors.at<double>(0),
							 -rot_mat.at<double>(1,0), -rot_mat.at<double>(1,1), -rot_mat.at<double>(1,2), -translationVectors.at<double>(1),
							 -rot_mat.at<double>(2,0), -rot_mat.at<double>(2,1), -rot_mat.at<double>(2,2), -translationVectors.at<double>(2),
									   0.0f,                      0.0f,                     0.0f,                   1.0f };
				modelview = glm::transpose(modelview);

			}
		}
		if (markerIds2.size() > 0) {
			// Try to find markers
			int valid2 = estimatePoseBoard(markerCorners2, markerIds2, board2, intrinsic_matrix, distortionParameters, rotationVectors2, translationVectors2);

			// If OpenCV finds it, then draw the model on top of it.
			if (valid2 > 0) {
				cv::Rodrigues(rotationVectors2, rot_mat2);

				modelview2 = { rot_mat2.at<double>(0,0),  rot_mat2.at<double>(0,1),  rot_mat2.at<double>(0,2),  translationVectors2.at<double>(0),
							 -rot_mat2.at<double>(1,0), -rot_mat2.at<double>(1,1), -rot_mat2.at<double>(1,2), -translationVectors2.at<double>(1),
							 -rot_mat2.at<double>(2,0), -rot_mat2.at<double>(2,1), -rot_mat2.at<double>(2,2), -translationVectors2.at<double>(2),
									   0.0f,                      0.0f,                     0.0f,                   1.0f };
				
			
				mvLuna = glm::transpose(modelview2);
				mvTIerra = mvLuna;
			}
		}

		//  --------------------------------------------------------------------------------------------------
		//  Draw Sol
		modelview = glm::scale(modelview, glm::vec3(0.05f, 0.05f, 0.05f));

		//  Light position
		glm::vec3 lamp_pos(1.0f, 1.0f, 0.0f);

		Sol.use();
		mvStack.push(modelview);
		mvStack.push(mvStack.top());
		mvStack.top() *= glm::translate(glm::mat4(1.0f),
			glm::vec3(2.5f, 3.0f, 3.0f));  // sun position
		mvStack.push(mvStack.top());  // duplicating
		mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)time,
			glm::vec3(0.0f, 0.0f, 1.0f));  // sun rotation
		mvStack.top() *= glm::scale(glm::mat4(1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f));  // make the sun bigger

		Sol.setMat4("model", model);
		Sol.setMat4("modelview", mvStack.top());
		Sol.setMat4("projection_perspective", projection_perspective);
		Sol.setVec3("ambient_light", ambient_light);
		Sol.setVec3("lamp_pos", lamp_pos);
		Sol.setVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
		Sol.setVec3("suzanne_color", glm::vec3(0.4f, 0.4f, 0.4f));
		Sol.setVec3("camera_pos", glm::vec3(0.0f, 0.0f, 0.0f));
		mSol.Draw(Sol);

		mvStack.pop();

		// --------------------------------------------------------------------------------
		// Draw Tierra
		mvTIerra = glm::scale(mvTIerra, glm::vec3(0.05f, 0.05f, 0.05f));
		mvTIerra = glm::translate(mvTIerra, glm::vec3(0.4f, 0.4f, 0.0f));
		/*mvTIerra = glm::rotate(glm::mat4(1.0f), 45,glm::vec3(0.0, 1.0, 0.0));*/
		// Light position
		glm::vec3 lamp_pos2(sin(time / 2.0f), cos(time / 2.0f), 0.0f);

		Tierra.use();
		mvStack2.push(mvTIerra);
		mvStack2.push(mvStack2.top());
		mvStack2.top() *= glm::translate(glm::mat4(1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f));  // planet position

		mvStack2.top() *= glm::translate(glm::mat4(1.0f),
			glm::vec3(2.5f, 3.0f, 3.0f));  // planet position
		mvStack2.push(mvStack2.top());  // duplicating
		mvStack2.top() *= glm::rotate(glm::mat4(1.0f), (float)time,
			glm::vec3(0.0, 0.0, 1.0));  // planet rotation
		mvStack2.top() *= glm::scale(glm::mat4(1.0f),
			glm::vec3(0.8f, 0.8f, 0.8f));  // make the Luna smaller

		Tierra.setMat4("model", mTierra1);
		Tierra.setMat4("modelview", mvStack2.top());
		Tierra.setMat4("projection_perspective", projection_perspective);
		Tierra.setVec3("ambient_light", ambient_light);
		Tierra.setVec3("lamp_pos", lamp_pos2);
		Tierra.setVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
		Tierra.setVec3("suzanne_color", glm::vec3(0.4f, 0.4f, 0.4f));
		Tierra.setVec3("camera_pos", glm::vec3(0.0f, 0.0f, 0.0f));
		mTierra.Draw(Tierra);

		mvStack2.pop();

		// --------------------------------------------------------------------------------
		// Draw Luna
		mvLuna = glm::scale(mvLuna, glm::vec3(0.05f, 0.05f, 0.05f));
		mvLuna = glm::translate(mvLuna, glm::vec3(0.2f, 0.2f, 0.0f));

		Luna.use();
		mvStack2.push(mvStack2.top());  // mvMat of planet
		mvStack2.top() *= glm::translate(glm::mat4(1.0f),
			glm::vec3(sin((float)time) * 2.0, cos((float)time) * 2.0,
				0.0f));  // Luna position
		mvStack2.push(mvStack2.top());  // duplicating
		mvStack2.top() *= glm::rotate(glm::mat4(1.0f), (float)time,
			glm::vec3(0.0, 0.0, 1.0));  // Luna rotation

		mvStack2.top() *= glm::scale(glm::mat4(1.0f),
			glm::vec3(0.50f, 0.50f, 0.50f));  // make the Luna smaller
		Luna.setMat4("model", mLuna1);
		Luna.setMat4("modelview", mvStack2.top());
		Luna.setMat4("projection_perspective", projection_perspective);
		Luna.setVec3("ambient_light", ambient_light);
		Luna.setVec3("lamp_pos", lamp_pos2);
		Luna.setVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
		Luna.setVec3("suzanne_color", glm::vec3(0.4f, 0.4f, 0.4f));
		Luna.setVec3("camera_pos", glm::vec3(0.0f, 0.0f, 0.0f));
		mLuna.Draw(Luna);

		mvStack2.pop();

		// ---------------------------------------------------------------------------------------------
		// Draw bagckground 
		glUseProgram(bgProgram);
		glBindVertexArray(VAO_bg);

		glActiveTexture(GL_TEXTURE2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
		glBindTexture(GL_TEXTURE_2D, texture_bg);
		glUniform1i(glGetUniformLocation(bgProgram, "webcam_texture"), 2);

		glUniformMatrix4fv(glGetUniformLocation(bgProgram, "modelview_bg"), 1, GL_FALSE, glm::value_ptr(modelview_bg));
		glUniformMatrix4fv(glGetUniformLocation(bgProgram, "perspective_projection_bg"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// ------------------------------------------------------------------------------------------
		// Draw lamp
		glm::mat4 model_lamp = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, lamp_pos.x, lamp_pos.y, 0.0f, 1.0f };
		model_lamp = glm::scale(model_lamp, glm::vec3(0.1f, 0.1f, 0.1f));

		glm::mat4 modelview_lamp = glm::mat4(1.0f);

		lampShader.use();
		lampShader.setMat4("model_lamp", model_lamp);
		lampShader.setMat4("modelview_lamp", modelview_lamp);
		lampShader.setMat4("projection_lamp", projection_perspective);
		lampModel.Draw(lampShader);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO_bg);
	glDeleteBuffers(1, &VBO_bg);
	mvStack.pop();

	mvStack.pop();
	mvStack.pop();
	mvStack.pop();

	glfwTerminate();
	return 0;
}



