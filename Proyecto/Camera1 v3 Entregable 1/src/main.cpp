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

int main(int argv, char** argc)
{
	Mat camaraMatrix = Mat::eye(3,3,CV_64F);

	Mat DistanciaCoeficiente;

	//ProcesoCalibracionCamara(camaraMatrix, DistanciaCoeficiente);

	CargarCamaraCalibracion("CalibraciondeCamara", camaraMatrix,DistanciaCoeficiente );

	WebcamMonitoreo(camaraMatrix, DistanciaCoeficiente, 0.099f);

        return 0;
}
