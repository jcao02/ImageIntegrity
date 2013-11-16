/*OPENCV LIBRARIES*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
/*STANDARD LIBRARIES*/
#include <iostream>

using namespace cv;
using namespace std;


/*Function to extract edges from an image as a signature*/
Mat edges_signature(Mat image) {
    Mat edges;
    
    return edges;
}


/*Function that generates signatures applying DCT transformation*/
int* generate_signature(Mat image) {
    return 0;
}


/*MAIN*/
int main(int argc, const char *argv[]) {
    Mat image, other_img, YCrCb_img;    // Matrices de imagenes (normal y escala de grises)
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

    other_img = image.rowRange(100, 200).colRange(100,200);

    imshow("Display window", other_img );
    waitKey(0);

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

    cout << YCrCb_img.size() << endl;

    
}
