#include <iostream>
#include <cstdlib>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctime>
#include <opencv2/objdetect/objdetect.hpp> //Libreria para deteccion de objetos
using namespace std;
using namespace cv;
 
 
Mat frame
Mat gx, gy; 
Mat decimal; 
Mat angulo;
Mat magnitud;
HOGDescriptor hog;
 
 
 
Mat hogRectangular (Mat frame, Mat &deteccion1){
    vector<Rect> deteccionesREClista;
    hog.detectMultiScale(frame, deteccionesREClista, 0, Size(16, 16), Size(128, 128), 1.05, 2, false);
    deteccion1 = frame.clone();
    Rect r;
    int cont = 0;
    int ancho= 0;
    int alto = 0;
 
    for (size_t i = 0; i < deteccionesREClista.size(); i++) {
        r = deteccionesREClista[i];
        ancho = r.width;
        alto = r.height;
        rectangle(deteccion1, r.tl(), r.br(), Scalar(10, 10, 237), 2);
        cont++;
        imwrite("Peatones.jpg", deteccion1);
    };
    cout << " HOG RECTANGULO : " << deteccionesREClista.size()<< endl;
    return deteccion1;
}
 
Mat hogCircular (Mat frame2, Mat &deteccion2) {
 
    Mat det2 = frame2.clone();
    Mat framegray;
    cvtColor(det2, framegray, COLOR_BGR2GRAY);
    medianBlur(framegray, framegray, 5);
    vector<Vec3f> listaDetCirculos;
 
    HoughCircles(framegray, listaDetCirculos, HOUGH_GRADIENT, 1, framegray.rows / 2, 105, 30, 1, 50);     
   
    for (size_t i = 0; i < listaDetCirculos.size(); i++) {
        Vec3i c = listaDetCirculos[i];
        Point center = Point(c[0], c[1]);
        int radio = c[2];
        circle(det2, center, radio, Scalar(10, 10, 237), 2);
    }
    cout << "HOG CIRCULAR: " << listaDetCirculos.size() << endl;
    return det2;
}
 
 
int main(int argc, char* argv[]) {
    //Presentacion del video
 
    Mat videoDetRec;
    Mat videoDetCir;
    Mat frameDeteccion;
  
 
    VideoCapture video("peatones.mp4");
    if (video.isOpened()) {
        namedWindow("Video Normal", WINDOW_AUTOSIZE);
        while (true) {
            video >> frame;
            if (frame.empty()) {
                break;
            }else {
                frame.convertTo(decimal, CV_32F, 1.0/255.0);
                Sobel(decimal, gx, CV_32F, 1, 0, 1); //Gradiente en X
                Sobel(decimal, gy, CV_32F, 0, 1, 1); //Gradiente en Y
                
                cartToPolar(gx, gy, magnitud, angulo, true); 
                
                hog = HOGDescriptor( Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9, 1 );
                hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
                videoDetRec = hogRectangular(frame, frameDeteccion);
                videoDetCir = hogCircular(frame, frameDeteccion);
                
                
                imshow("Video ORIGINAL", frame);
                imshow("Detecciones Rectangulares", videoDetRec);
                imshow("Detecciones Circulares", videoDetCir);
                imshow("Gradiente X", gx);
                imshow("Gradiente Y", gy);
                imshow("Magnitud", magnitud);
                imshow("Angulo", angulo);
                
               
                if (waitKey(10) == 27){
                    break;
                }   
            }
        }
        destroyAllWindows();
    }

};
