/*OPENCV LIBRARIES*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
/*STANDARD LIBRARIES*/
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, const char *argv[])
{
    Mat image, gray_img, YCrCb_img;    // Matrices de imagenes (normal y escala de grises)
    vector<Mat> channels;   // Vector de matrices para los canales de RGB
    vector<Mat> mod(3);   // Vector de matrices para los canales de RGB


    /* Cantidad de argumentos */
    if (argc != 2) {
        cout << "Must specify image" << endl;
        return -1;
    }

    /*Se lee la imagen*/
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    /*Si no se recibio ningun dato de la imagen*/
    if (!image.data) {
        cout << "Could not open or find image" << endl;
        return -1;
    }


    /*Creacion de imagen con escala de grises*/
    cvtColor( image, gray_img, CV_BGR2GRAY );



    /*Creacion de ventana para mostrar imagenes*/
    namedWindow("Display window", CV_WINDOW_AUTOSIZE);
    
    imshow("Display window", gray_img );
    waitKey(0);

    /*Separacion de canales*/
    cv::split(image, channels);

    imshow("Display window",  channels[0]);
    waitKey(0);

    imshow("Display window",  channels[1]);
    waitKey(0);

    imshow("Display window",  channels[2]);
    waitKey(0);

/*

    mod[0] = channels[1];
    mod[1] = channels[0];
    mod[2] = channels[2];

    cv::merge(mod, image);
    imshow("Display window", image );
    waitKey(0);

*/



    /*Creacion de imagen a formato YCrCb*/
    cvtColor(image,YCrCb_img,CV_BGR2YCrCb);

    imshow("Display window", YCrCb_img );
    waitKey(0);

    /*Separacion de canales*/
    cv::split(YCrCb_img, channels);

    /*Muestra en pantalla las imagenes*/

    imshow("Display window",  channels[0]);
    waitKey(0);

    imshow("Display window",  channels[1]);
    waitKey(0);

    imshow("Display window",  channels[2]);
    waitKey(0);

    
}
