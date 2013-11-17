/*OPENCV LIBRARIES*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
/*STANDARD LIBRARIES*/
#include <iostream>
#include <vector>

/*Definitions*/
#define THRESHOLD 0.7
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
vector< vector<int> > generate_signature(Mat image) {
    Mat a, b, tmp,dct1, dct2;
    vector< vector<int> > result;
    vector<int> vrow;
    int i, j, n, m, cmp = 1,
        row, col;
    double minVal, maxVal;
    Point minLoc, maxLoc;

    // Size of the matrix
    n = image.size().width;
    m = image.size().height;

    // Initialize vector
    result = std::vector< vector<int> > (n / 8);
   
    row = 0;
    // for to compare 4 size blocks of the image
    for ( i = 0; i < n - 4 && row < n / 8; i += 4 ) {
        vrow = std::vector< int > (m / 4);
        col = 0;
        for ( j = 0; j < m - 8 && col < m / 4; j += 8) {
            a = image.rowRange(i, i + 4).colRange(j, j + 4);        // Subimage a size 4x4 
            b = image.rowRange(i, i + 4).colRange(j + 4, j + 8);    // Subimage b size 4x4 

            dct(a, dct1, 0);    //DCT transformation on image a
            dct(b, dct2, 0);    //DCT transformation on image b

            /*Comparing DCT's transformations*/
            
            matchTemplate(dct1, dct2, tmp, 3);
            minMaxLoc( tmp, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

            if ( maxVal > THRESHOLD ) {
                vrow[col++] = 1;
            } else {
                vrow[col++] = 0;
            }
        }
        result[row] = vrow;
        ++row;
    }
            
    return result;
}

/*Function that compares signatures between two images*/
bool compare_signatures(Mat image1, Mat image2, int type)
{
    vector< vector<int> > signature1, signature2;
    Mat YCrCb_img1, YCrCb_img2,
        edges1, edges2, result;
    vector<Mat> channels1, channels2;  
    double maxVal, minVal;
    Point minLoc, maxLoc;



    if (type == 1) {

        edges1 = edges_signature(image1);
        edges2 = edges_signature(image2);
   
        matchTemplate(edges1, edges2, result, 3);
        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

        if ( maxVal > THRESHOLD ) {
            return true;
        }

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

        /*Generating signatures*/
        signature1 = generate_signature(channels1[0]);
        signature2 = generate_signature(channels2[0]);
 
        if (DEBUG){
            cout << "Image 1 ---------------------------------------------------------------------------------------------" << endl;
            for (std::vector< vector<int> >::const_iterator elem = signature1.begin(); elem != signature1.end(); ++elem) {
                vector<int> temp;
                temp = *elem;
                for (std::vector<int>::const_iterator k = temp.begin(); k != temp.end(); ++k)
                    cout << *k << " ";
                cout << endl;
            }

            cout << "Image 2 ---------------------------------------------------------------------------------------------" << endl;
            for (std::vector< vector<int> >::const_iterator elem = signature2.begin(); elem != signature2.end(); ++elem) {
                vector<int> temp;
                temp = *elem;
                for (std::vector<int>::const_iterator k = temp.begin(); k != temp.end(); ++k)
                    cout << *k << " ";
                cout << endl;
            }
        }

        /*Comparing signatures*/
        //DETERMINE WHAT CRITERIUM USE TO COMPARE THE SIGNATURE


    }

    return false;
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

    if ( compare_signatures(image1, image2, type) )

        cout << "Same image" << endl;
    else

        cout << "Different image" << endl;

}
