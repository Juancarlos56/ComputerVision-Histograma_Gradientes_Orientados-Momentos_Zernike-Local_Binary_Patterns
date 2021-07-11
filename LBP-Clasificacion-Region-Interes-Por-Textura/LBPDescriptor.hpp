#include <iostream>
#include <cstdlib>
#include <cstring>

#include <cmath>

// Librerías de OpenCV

//#include <opencv2/opencv.hpp>

#include <opencv2/core/core.hpp> // Contiene los elementos base de OpenCV (matrices, arreglos, etc.)
#include <opencv2/highgui/highgui.hpp> // Contiene estructuras para crear la interfaz gráfica
#include <opencv2/imgproc/imgproc.hpp> // Procesamiento de imágenes
#include <opencv2/imgcodecs/imgcodecs.hpp> // Códecs para leer determinados formatos de imágenes
#include <opencv2/video/video.hpp> // Procesamiento y escritura
#include <opencv2/videoio/videoio.hpp> // de video

#include <opencv2/objdetect/objdetect.hpp> // Librería para detección de objetos (contiene el HOG Descriptor)

using namespace std;

using namespace cv; // El espacio de nombres donde están definidas algunas funciones de OpenCV


#define compab_mask_inc(ptr,shift) \
{ value |= ((unsigned int)(cntr - *ptr) & 0x80000000) \
>> (31-shift); ptr++; }


class LBPDescriptor{
    private:
        int* LBP8(const int*, int, int);

    public:
        LBPDescriptor();
        vector<int> lbp(Mat);
};

