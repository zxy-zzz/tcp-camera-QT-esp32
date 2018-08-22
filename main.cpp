#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Dialog w;
    w.show();

    return a.exec();
}



//#include <QApplication>
//#include <iostream>

//#include <opencv2/opencv.hpp>

//using namespace std;
//using namespace cv;
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    Mat image = imread("D:\\IMG_20161205_114535.jpg");
//    cvNamedWindow("window", CV_WINDOW_NORMAL);
//    imshow("window",image);
//    waitKey(1000);

//    return a.exec();
//}

