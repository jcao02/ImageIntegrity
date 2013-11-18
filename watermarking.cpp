/*OPENCV LIBRARIES*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
/*STANDARD LIBRARIES*/
#include <iostream>
#include <vector>
#include <algorithm>

/*Definitions*/
#define THRESHOLD_ALTERATION 0.12
#define DEBUG 0
/*Namespaces*/
using namespace cv;
using namespace std;


/*Function to extract edges from an image as a signature*/
Mat edges_signature(Mat image) {
    Mat edges;
    
    Canny(image, edges, 20, 20*3);

    return edges;
}

/*Function that generates signatures applying DCT transformation*/
Mat generate_signature(Mat image) {
    Mat a, b, tmp,dct1, dct2;
    Mat result;
    int i, j, n, m, cmp = 1,
        row, col;
    float coef1, coef2;

    // Size of the matrix
    n = image.rows;
    m = image.cols;

    result = cv::Mat::zeros(n / 8, m / 4, CV_32FC1);
    row = 0;
    // for to compare 4 size blocks of the image
    for ( i = 0; i < n - 4 && row < n / 8; i += 4 ) {
        col = 0;
        for ( j = 0; j < m - 8 && col < m / 4; j += 8) {
            a = image.rowRange(i, i + 4).colRange(j, j + 4);        // Subimage a size 4x4 
            b = image.rowRange(i, i + 4).colRange(j + 4, j + 8);    // Subimage b size 4x4 

            dct(a, dct1, 0);    //DCT transformation on image a
            dct(b, dct2, 0);    //DCT transformation on image b

            coef1 = (float) dct1.at<float>(0,0);
            coef2 = (float) dct2.at<float>(0,0);
            //cout << dct1 << " CON " << dct2 << endl;
            /*Comparing DCT with first DC coefficient*/
            if (coef1 > coef2) { 
                result.at<float>(row, col++) = 1.0;
            } else {
                result.at<float>(row, col++) = 0.0;
            }
        }
        ++row;
    }


    return result;
}

/*Locates modifications from image and shows them in the original one*/
void show_locations(Mat image, Mat xor_matrix, int type)
{
    int x, y, i, j,
        rows, cols, size, dif;
    Mat act;
    Point p1, p2;
    float result;

    rows = xor_matrix.rows;
    cols = xor_matrix.cols;
    x = rows / 10;
    y = cols / 10;
    size = x * y;
    result = (float) 560 / size;

    namedWindow("Image alteration");

    for (i = 0; i < rows && i + x < rows; i += x) {
        for (j = 0; j < cols && j + y < cols; j += y) {
            act = xor_matrix.rowRange(i, i + x).colRange(j, j + y);
            dif = countNonZero(act);
            result = (float) dif / size;
            if (result > THRESHOLD_ALTERATION) {
                if (type == 1) {
                    p1 = Point(i,j);
                    p2 = Point(i + x, j + y);
                } else {
                    p1 = Point(i * 8,j * 4);
                    p2 = Point((i + x) * 8, (j + y) * 4);
                }
                rectangle(image, p1, p2, Scalar(0,0,255));

            }

        }
    }

    imshow("Image alteration", image);
    waitKey(0);
}

/*Function that compares signatures between two images*/
void compare_signatures(Mat image1, Mat image2, int type)
{
    Mat signature1, signature2, differenceM;
    Mat YCrCb_img1, YCrCb_img2, clon;
    vector<Mat> channels1, channels2;  
    int diff;

    clon = image1.clone();

    /*Type 1 = Edges, Type 2 = DCT*/
    if (type == 1) {

        signature1 = edges_signature(image1);
        signature2  = edges_signature(image2);

    } else {
        /* Changing image scale*/
        image1.convertTo(image1, CV_32FC1);
        image2.convertTo(image2, CV_32FC1);

        /*Creating images YCrCb*/
        cvtColor(image1,YCrCb_img1,CV_BGR2YCrCb);
        cvtColor(image2,YCrCb_img2,CV_BGR2YCrCb);

        /*Separating channels of each YCrCb image*/
        cv::split(YCrCb_img1, channels1);
        cv::split(YCrCb_img2, channels2);

        /*Generating signatures with DCT*/
        signature1 = generate_signature(channels1[0]);
        signature2 = generate_signature(channels2[0]);
    }

    /*XOR to get missmatches between images*/
    differenceM = signature1 ^ signature2;

    /*Quantity of missmatches*/
    diff = countNonZero(differenceM);

    /*Same image, nothing to do*/
    if (diff == 0) {
        cout << "Exact same image, nothing to compute" << endl;
        return;
    }

    if (DEBUG) {
        namedWindow("Debug");
        imshow("Debug",clon);
        waitKey(0);
    }
    

   /*Show image alterations*/
    show_locations(clon, differenceM, type);


}

/*Print options to user*/
void print_options()  
{
    cout << "Select type of signature to use:" << endl;
    cout << "(1) Egdes signature" << endl;
    cout << "(2) DCT signature" << endl;
    cout << "Your option: ";
}

/*Get input from user*/
int get_input()
{
    int selection;
    print_options();
    cin >> selection;

    if ( selection == 1  || selection == 2 ) {

        return selection;
    } else {
    

        cout << endl << "Selected option not valid!" << endl;
        cout << "--------------------------------" << endl << endl;
        get_input();
    }
}


/*MAIN*/
int main(int argc, const char *argv[]) {
    Mat image1, image2;  
    vector<Mat> channels1, channels2;  
    int type;
    Size s;


    /* Cantidad de argumentos */
    if (argc != 3) {
        cout << "Usage: ./watermarking image1 image2" << endl;
        return -1;
    }

    /*Se lee la imagen*/
    image1 = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    image2 = imread(argv[2], CV_LOAD_IMAGE_COLOR);

    /*Si no se recibio ningun dato de la imagen*/
    if (!image1.data || !image2.data) {
        cout << "Could not open or find image" << endl;
        return -1;
    }


    if (DEBUG) {
        namedWindow("Debug");
        imshow("Debug", image1);
        waitKey(0);
        imshow("Debug", image2);
        waitKey(0);
    }
    /*Resizing image2 to image1 size*/
    s = cv::Size(image1.cols, image1.rows);
    resize(image2, image2, s);

    /*Get the input from the user*/
    type = get_input();

    compare_signatures(image1, image2, type);

}


// XOR between signatures
// BER between two signatures (to check if its alterated)
// Subdivide result in 9x9 subblocks (size might change depending on image size) 
// block > 50% means is alterted
