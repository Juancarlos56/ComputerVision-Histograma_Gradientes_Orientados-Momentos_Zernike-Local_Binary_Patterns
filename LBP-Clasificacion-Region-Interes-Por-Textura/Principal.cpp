//Importacion de librerias para el proyecto
#include "LBPDescriptor.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <list>
#include <iterator>
#include <stdio.h>
#include <string.h>
#include <fstream>

//Funcion que nos permite calcular la distancia euclidea entre dos imagenes
double distanciaEuclidea(vector<int> *lbpOriginal, vector<int> *lbpCompare){
    
    double distancia, resta = 0;

    for (int i=0;i<256;i++){    //Bucle para restar componentes y elevarlas a 2
        resta=resta+pow(lbpOriginal->at(i)- lbpCompare->at(i),2);
    }
    return distancia=sqrt(resta);
    
}


//Creacion de una clase para almacenar resultados
class Categoria
{
private:
    //PATH de la imagen 
    string identificador;
    //Distancia euclidea resultante mas pequena
    double distancia;
    //A que categoria segun la clasificacion del algoritmo
    int categoria;
    // Valor real a la clase que pertenece
    int clasificador;

public:

    Categoria(double distancia, int categoria, string identificador, int clasificador){
        this->distancia = distancia;
        this->categoria = categoria;
        this->identificador = identificador;
        this->clasificador = clasificador;
    }


    double getDistancia() {
      return distancia;
    }
    
    int getClasificador() {
      return clasificador;
    }

    int getCategoria() {
      return categoria;
    }

    string getIdentificador() {
      return identificador;
    }
};


int main(int argc, char *argv[]){
    
    //Llamada y instancia de objeto para hacer uso de la funcion lbp
    LBPDescriptor *lbpd = new LBPDescriptor();

    //Inicializacion de variables a utilizar
    Mat imgCompare;
    Mat imgCompareCielab;
    Mat imgGRAYCompare;
    
    Mat img;
    Mat imgCielab;
    Mat imgGRAY;

    //Iinicializacion de listas para almacenar resultados
    list <Categoria> listaDistanciaCategoria;
    list <Categoria> listaClasificacionMenorDistancia;
    list <Categoria> listaClasificacionCategoriaResultados;

    //Inicializacion de vectores para alamacenar histograma LBP
    vector<int> descriptorLBP_imgCompare;
    vector<int> descriptorLBP_imgOriginal;

    /* Con un puntero a DIR abriremos el directorio */
    DIR *directorioImagenes, *directorioImagenesTrain;
    /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
    struct dirent *ent, *entDirectorioImagenes, *entDirectorioImagenesTrain;

    const char *PATHS[4] = { "images/banded/", "images/chequered/","images/cracked/", "images/crystalline/"};
    const char *PATHS_Test[4] = { "images/test_Imagenes/banded/", "images/test_Imagenes/chequered/","images/test_Imagenes/cracked/", "images/test_Imagenes/crystalline/"};
    
    //Categorias de clasificacion
    const char *Categorias[4] = {"banded", "chequered", "cracked", "crystalline"};

    
    for (int i = 0; i < 4; i++){
        // Empezaremos a leer en el directorio actual 
        directorioImagenes = opendir (PATHS_Test[i]);
        
        // Miramos que no haya error 
        if (directorioImagenes == NULL){
            cout<<"No puedo abrir el directorio: "<<PATHS_Test[i]<<endl;
            
        }

        // Leyendo uno a uno todos los archivos que hay dentro del directorio
        while (((entDirectorioImagenes = readdir (directorioImagenes)) != NULL) ){
            
            // Nos devolverá el directorio actual (.)  
            if ( strcmp(entDirectorioImagenes->d_name, PATHS_Test[i]) < 0 ){
                
                char *pathImagenDirectorioActual = entDirectorioImagenes->d_name;
                string pathImagenDirectorioActualString(pathImagenDirectorioActual);

                if ((pathImagenDirectorioActualString != ".")&&(pathImagenDirectorioActualString != "..")){
                    
                    //cout<<"<<<<<<<<<<<<<<<<<" <<PATHS_Test[i]+pathImagenDirectorioActualString <<">>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
                    //lECUTA de imagen a ser comparada, imagen original del test
                    img = imread(PATHS_Test[i]+pathImagenDirectorioActualString);
                    //resize(img, img, cv::Size(), 0.75, 0.75);
                    //Pasamos la imagen al espacio de color CIELAB
                    cvtColor(img, img, COLOR_RGB2Lab);
                    //Convertimos del espacio de color LAB al espacio de color GRAY
                    cvtColor(img, imgGRAY, COLOR_RGB2GRAY);
                    //Obtencion de matriz LBP de la imagen original
                    descriptorLBP_imgOriginal = lbpd->lbp(imgGRAY);
                    

                    for (int j = 0; j < 4; j++){
                        int aux = 10000000;
                        // Empezaremos a leer en el directorio actual 
                        directorioImagenesTrain = opendir (PATHS[j]);
                        // Miramos que no haya error 
                        if (directorioImagenesTrain == NULL){
                            cout<<"No puedo abrir el directorio: "<<PATHS[j]<<endl;
                            
                        }
                        // Leyendo uno a uno todos los archivos que hay dentro del directorio
                        while (((entDirectorioImagenesTrain = readdir (directorioImagenesTrain)) != NULL) ){
                            if ( strcmp(entDirectorioImagenesTrain->d_name, PATHS[j]) < 0 ){
                                char *pathImagenDirectorioComparacion = entDirectorioImagenesTrain->d_name;
                                string pathImagenDirectorioComparacionString(pathImagenDirectorioComparacion);

                                if ((pathImagenDirectorioComparacionString != ".")&&(pathImagenDirectorioComparacionString != "..")){
                                    
                                    //cout<<"----------------" <<PATHS[j]+pathImagenDirectorioComparacionString <<"----------------"<<endl;
                                    //Lecura de imagen a ser comparada con la original
                                    imgCompare = imread(PATHS[j]+pathImagenDirectorioComparacionString);
                                    //resize(imgCompare, imgCompare, cv::Size(), 0.75, 0.75);
                                    //Igualmente pasamos al espacio de color cielab
                                    cvtColor(imgCompare, imgCompare, COLOR_RGB2Lab);
                                    //Pasamos al espacio de color gray desde cielab
                                    cvtColor(imgCompare, imgGRAYCompare, COLOR_RGB2GRAY);
                                    //Obtencion de matriz LBP de la imagen original
                                    descriptorLBP_imgCompare = lbpd->lbp(imgGRAYCompare);
                                    //Obtencion de la distancia euclidea 
                                    double distanciaEuclideaLBP = distanciaEuclidea(&descriptorLBP_imgOriginal, &descriptorLBP_imgCompare);
                                    //comprobacion del menor valor de la distancia
                                    if(distanciaEuclideaLBP < aux){
                                        aux = distanciaEuclideaLBP;
                                        //cout<<"Valor minimo: "<< aux<<endl;
                                    }
                                }
                            }
                        }
                        
                        int claseImagen;

                        if(PATHS_Test[i] == "images/test_Imagenes/banded/"){
                            claseImagen = 0;
                        }else if(PATHS_Test[i] == "images/test_Imagenes/chequered/"){
                            claseImagen = 1;
                        }else if(PATHS_Test[i] == "images/test_Imagenes/cracked/"){
                            claseImagen = 2;
                        }else if(PATHS_Test[i] == "images/test_Imagenes/crystalline/"){
                            claseImagen = 3;
                        }

                        int claseImagenPrediccion;
                        if(Categorias[j] == "banded"){
                            claseImagenPrediccion = 0;
                        }else if(Categorias[j] == "chequered"){
                            claseImagenPrediccion = 1;
                        }else if(Categorias[j] == "cracked"){
                            claseImagenPrediccion = 2;
                        }else if(Categorias[j] == "crystalline"){
                            claseImagenPrediccion = 3;
                        }

                        //Creacion de objetos de tipo categoria para ver las distancia en cada imagen
                        Categoria categoriaObtenida(aux, claseImagenPrediccion, PATHS_Test[i]+pathImagenDirectorioActualString, claseImagen);
                        listaDistanciaCategoria.push_back(categoriaObtenida);
                        closedir (directorioImagenesTrain);
                    }


                    list <Categoria> :: iterator it;
                    for(it = listaDistanciaCategoria.begin(); it != listaDistanciaCategoria.end(); ++it){
                        if (listaClasificacionMenorDistancia.empty())
                        {
                            listaClasificacionMenorDistancia.push_back(*it);
                        }else if(listaClasificacionMenorDistancia.front().getDistancia() > it->getDistancia()){
                                listaClasificacionMenorDistancia.pop_front();
                                listaClasificacionMenorDistancia.push_back(*it);
                        }
                        //cout  << '\t' <<it->getDistancia() << ":"<< it->getCategoria()<< " : "<<it->getIdentificador()<<'\t';   
                    }

                    for(it = listaClasificacionMenorDistancia.begin(); it != listaClasificacionMenorDistancia.end(); ++it){
                        listaClasificacionCategoriaResultados.push_back(*it);
                    }
                    
                    listaDistanciaCategoria.clear();
                    listaClasificacionMenorDistancia.clear();
                   
                }   
            } 
        }
        
    }

    closedir (directorioImagenes);
    int contadoAciertosTotales = 0;
    int contadoFallosTotales = 0;
    int contadorAciertos0 = 0;
    int contadorFallos0 = 0;
    int contadorAciertos1 = 0;
    int contadorFallos1 = 0;
    int contadorAciertos2 = 0;
    int contadorFallos2 = 0;
    int contadorAciertos3 = 0;
    int contadorFallos3 = 0;


     
    list <Categoria> :: iterator it;
    for(it = listaClasificacionCategoriaResultados.begin(); it != listaClasificacionCategoriaResultados.end(); ++it){
        //cout  << '\t' <<it->getDistancia() << ":"<< it->getCategoria()<< " : "<<it->getIdentificador()<< " : "<<it->getClasificador()<<'\t';   
        if(it->getClasificador() == 0){
            if(it->getClasificador() == it->getCategoria()){
                contadorAciertos0++;
            }else {
                contadorFallos0++;
            }
        }else if(it->getClasificador() == 1){
            if(it->getClasificador() == it->getCategoria()){
                contadorAciertos1++;
            }else {
                contadorFallos1++;
            }
        }else if(it->getClasificador() == 2){
            if(it->getClasificador() == it->getCategoria()){
                contadorAciertos2++;
            }else {
                contadorFallos2++;
            }
        }else if(it->getClasificador() == 3){
            if(it->getClasificador() == it->getCategoria()){
                contadorAciertos3++;
            }else {
                contadorFallos3++;
            }
        }
        
        if(it->getClasificador() == it->getCategoria()){
            contadoAciertosTotales++;
        }else {
            contadoFallosTotales++;
        }

    }
    //cout << endl;
    
    cout<<"---------------------RESULTADOS--------------------------------"<<endl;
    cout<<"<<<<<<<<<<<PRESICION DEL CLASIFICADOR LBP>>>>>>>>>>>>>>>>"<<endl;
    cout<< "Size del descriptor LBP: "<<descriptorLBP_imgOriginal.size()<<endl;

    cout<<"Clasificados correctamente: "<<contadoAciertosTotales<<endl;
    cout<<"Clasificados Incorrectamente: "<<contadoFallosTotales<<endl;
    cout<<"Precision del modelo: "<<to_string( ( (double)(contadoAciertosTotales)*(100.00))/(double)(contadoAciertosTotales + contadoFallosTotales)) <<endl;

    cout<<"<<<<<<PRESICION DEL CLASIFICADOR LBP POR CATEGORIAS>>>>>>"<<endl;
    cout<<"<<<<<<PRESICION DEL PARA LA CATEGORIA BANDED>>>>>>"<<endl;
    cout<<"Clasificados correctamente: "<<contadorAciertos0<<endl;
    cout<<"Clasificados Incorrectamente: "<<contadorFallos0<<endl;
    cout<<"Precision del modelo: "<<to_string( ( (double)( contadorAciertos0)*(100.00))/(double)(contadorAciertos0 + contadorFallos0)) <<endl;

    cout<<"<<<<<<PRESICION DEL PARA LA CATEGORIA CHEQUERED>>>>>>"<<endl;
    cout<<"Clasificados correctamente: "<<contadorAciertos1<<endl;
    cout<<"Clasificados Incorrectamente: "<<contadorFallos1<<endl;
    cout<<"Precision del modelo: "<<to_string( ( (double)(contadorAciertos1)*(100.00))/(double)(contadorAciertos1 + contadorFallos1)) <<endl;

    cout<<"<<<<<<PRESICION DEL PARA LA CATEGORIA CRACKED>>>>>>"<<endl;
    cout<<"Clasificados correctamente: "<<contadorAciertos2<<endl;
    cout<<"Clasificados Incorrectamente: "<<contadorFallos2<<endl;
    cout<<"Precision del modelo: "<<to_string( ( (double)(contadorAciertos2)*(100.00))/(double)(contadorAciertos2 + contadorFallos2)) <<endl;

    cout<<"<<<<<<PRESICION DEL PARA LA CATEGORIA CRYSTALLINE>>>>>>"<<endl;
    cout<<"Clasificados correctamente: "<<contadorAciertos3<<endl;
    cout<<"Clasificados Incorrectamente: "<<contadorFallos3<<endl;
    cout<<"Precision del modelo: "<<to_string( ( (double)(contadorAciertos3)*(100.00))/(double)(contadorAciertos3 + contadorFallos3)) <<endl;
    
    
    
    // Representación gráfica 
    int *grafica = new int[600];

    for(int i=0;i<600;i++){
        if (i==30){
            grafica[i] = (double)contadorAciertos0 + contadorFallos0;
        }else if (i==70){
            grafica[i] = (double)contadorAciertos0;
        }else if (i==110){
            grafica[i] = (double)contadorFallos0;
        }else if (i == 170){
            grafica[i] = (double)contadorAciertos1 + contadorFallos1;
        }else if (i == 210){
            grafica[i] = (double)contadorAciertos1;
        }else if (i == 250){
            grafica[i] = (double)contadorFallos1;
        }else if (i == 320){
            grafica[i] = (double)contadorAciertos2 + contadorFallos2;
        }else if (i == 360){
            grafica[i] = (double)contadorAciertos2;
        }else if (i == 400){
            grafica[i] = (double)contadorFallos2;
        }else if (i == 470){
            grafica[i] = (double)contadorAciertos3 + contadorFallos3;
        }else if (i == 510){
            grafica[i] = (double)contadorAciertos3;
        }else if (i == 550){
            grafica[i] = (double)contadorFallos3;
        }else{
            grafica[i] = 0;
        }
        
        
        
    }
        

    int altoLienzo = 600;
    int anchoLienzo = 600;
    Mat lienzo = Mat(Size(anchoLienzo, altoLienzo),  CV_8UC3, Scalar(255, 255, 255));
    
    double escala = 12;
    cout << "Máximo valor histograma: " << (double) descriptorLBP_imgOriginal.size() << " Escala: " << escala << endl;
    
    rectangle(lienzo,Point(10, 10), Point(590, 598), Scalar(0,0,0), 1,  LINE_8);
    
    int cont = 0;
    for (int i=0, j=0;i<600;i++,j++){
        if ((cont == 0) && (grafica[i] !=0)){
            rectangle(lienzo,Point(j, altoLienzo), Point(j+40,((double)altoLienzo)-(escala*((double)grafica[i]))), Scalar(191,132,36), FILLED,  LINE_8);
            cv::putText(lienzo,std::to_string(grafica[i]),Point(j+10,(((double)altoLienzo)-(escala*((double)grafica[i])))-15),cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,0,0),1,false);
            cont++;
        }else if(cont == 1  && grafica[i] !=0 ){
            rectangle(lienzo,Point(j, altoLienzo), Point(j+40,((double)altoLienzo)-(escala*((double)grafica[i]))), Scalar(15, 98, 242), FILLED,  LINE_8);
            cv::putText(lienzo,std::to_string(grafica[i]),Point(j+10,(((double)altoLienzo)-(escala*((double)grafica[i])))-15),cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,0,0),1,false);
            cont++;
        }else if(cont ==2  && grafica[i] !=0 ){
            rectangle(lienzo,Point(j, altoLienzo), Point(j+40,((double)altoLienzo)-(escala*((double)grafica[i]))), Scalar(46,166,50), FILLED,  LINE_8);
            cv::putText(lienzo,std::to_string(grafica[i]),Point(j+10,(((double)altoLienzo)-(escala*((double)grafica[i])))-15),cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,0,0),1,false);
            cont=0;
        }else{
            rectangle(lienzo,Point(j, altoLienzo), Point(j+40,((double)altoLienzo)-(escala*((double)grafica[i]))), Scalar(240,240,10), FILLED,  LINE_8);
        }
        
    }
    cv::putText(lienzo,"Clasificador para cada Categoria",cv::Point(30,35),cv::FONT_HERSHEY_DUPLEX,1,cv::Scalar(0,0,0),2,false);
    cv::putText(lienzo,"Precicion del Modelo:"+ (to_string( ( (double)(contadoAciertosTotales)*(100.00))/(double)(contadoAciertosTotales + contadoFallosTotales)) )
        ,cv::Point(30, 70),cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,0,0),1,false);

    cv::putText(lienzo,"Valores Reales",cv::Point(400,70),cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(191,132,36),1,false);
    cv::putText(lienzo,"Prediccion-Aciertos",cv::Point(400,90),cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(15, 98, 242),1,false);
    cv::putText(lienzo,"Prediccion-Fallos",cv::Point(400,110),cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(46,166,50),1,false);
    

    namedWindow("ComparacionEspacioColor",WINDOW_AUTOSIZE);
    imwrite("Resultados-Clasificacion-LBP.jpg", lienzo);
    imshow("ComparacionEspacioColor", lienzo);
  
    waitKey(0);
    cv::destroyAllWindows();

    return 0;
}





