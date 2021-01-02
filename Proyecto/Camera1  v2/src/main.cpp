#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/calib3d/calib3d_c.h>

#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

const float calibrationSquareDimesion = 0.01905f; //metros
const float arucoSquareDimension = 0.1016f; //metros
const Size chessboardDimensions = Size(6, 9);

void crearArucoMarkers()
{
    Mat outputMarker;

    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

    for(int i = 0; i < 50; i++)
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


int main(int argv, char** argc)
{
	Mat Frame;
	Mat DibujarFrame;

	Mat camaraMatrix = Mat::eye(3,3,CV_64F);

	Mat DistanciaCoeficiente;

	vector<Mat> GuardarImagen;

	vector<vector<Point2f>> EsquinasMarcador, CandidatosRecha;

	VideoCapture vid(0);

	if(!vid.isOpened())
	{
		return 0;
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
	}


	//crearArucoMarkers();

        return 0;
}
