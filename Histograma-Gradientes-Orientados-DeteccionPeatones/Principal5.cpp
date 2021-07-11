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

//Variables Globales
Mat frame;
Mat decimal; //Imagen en formato decimal para los gradientes
Mat gx, gy; //Graditnes en x y
Mat angulo, magnitud;// angulo y magnitud del gradiente
Mat anterior;
Mat resta;

int winSize = 4;
int paddingSize = 4;

int main(int argc, char* argv[]) {
    //Presentacion del video
    VideoCapture video("personas-caminando.mp4");
    if (video.isOpened()) {
        namedWindow("Video", WINDOW_AUTOSIZE);
        while (3 == 3) {
            video >> frame;
            if (frame.rows == 0 || frame.cols == 0) {
                break;
            }else {
                resize(frame, frame, Size(), 0.5, 0.5);

                //Normalizacion de imagen escala entre 0 y 1 (255=1.0)
                frame.convertTo(decimal, CV_32F, 1.0 / 255.0); 
                //Calculo de los bordes mediante sobel
                Sobel(decimal, gx, CV_32F, 1, 0, 1); //Gradiente en X
                Sobel(decimal, gy, CV_32F, 0, 1, 1); //Gradiente en Y
                //Calculo de la magnitud y el angulo
                cartToPolar(gx, gy, magnitud, angulo, true); //Se pasan a cordenadas Polares

                //Calculo del histograma de HOG
                HOGDescriptor hog;
                hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
                vector<Rect> detecciones;
                hog.detectMultiScale(frame, detecciones, 0, Size(8, 8), Size(128,128), 1.5, 2, false);
                
                Mat clone = frame.clone();
                cout << " Se detectaron una cantidad de : " << detecciones.size() <<" Peatones. " << endl;
                
                Rect r;
                int cont = 0;

                for (size_t i = 0; i < detecciones.size(); i++) {
                    r = detecciones[i];
                    r.x += cvRound(r.width * 0.1);
                    r.width = cvRound(r.width * 0.8);
                    r.y += cvRound(r.height * 0.073);
                    r.height = cvRound(r.height * 0.8);
                    
                    rectangle(clone, r.tl(), r.br(), Scalar(10, 10, 237), 2);
                    
                    cont++;
                    imwrite("Peatones.jpg", clone);
                };
    
                std::ostringstream vts;
                vts << detecciones.size();
                string texto = "N° de peatones " + vts.str();
                cout << vts.str() << endl;
                
                
                //Cálculo del HOG circular 
                Mat clone2 = frame.clone();
                Mat gray;
                cvtColor(clone2, gray, COLOR_BGR2GRAY);
                medianBlur(gray, gray, 5);
                vector<Vec3f> circles;
                HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows / 2, 105, 30, 1, 50);
                
                cout << "Detecciones Circulares: " << circles.size() << endl;
                for (size_t i = 0; i < circles.size(); i++) {
                    Vec3i c = circles[i];
                    Point center = Point(c[0], c[1]);
                // circle center
                //circle( frame, center, 1, Scalar(0,300,300), 3, LINE_AA);
                // circle outline
                int radius = c[2];
                circle(clone2, center, radius, Scalar(10, 10, 233), 2,
                LINE_AA);
                }
                
                putText(clone, texto, Point(30, 50), FONT_HERSHEY_PLAIN, 2, Scalar(0,
                255, 0), 3);
                imshow("Video", frame);
                imshow("Gradiente X", gx);
                imshow("Gradiente Y", gy);
                imshow("Magnitud", magnitud);
                imshow("Detecciones", clone);
                imshow("Circulos", clone2);
                imshow("Angulo", angulo);
                //imshow("Nueva", nueva);
                //("Gris",resta);
                //imshow("Resultado", resultado);
                if (waitKey(10) == 27){
                    break;
                }   
            }
        }
        destroyAllWindows();
    }
};