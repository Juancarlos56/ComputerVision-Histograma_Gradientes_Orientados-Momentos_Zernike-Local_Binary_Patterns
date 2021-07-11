# Computer Vision para la identifiacion de Momentos Zernike

## 3. Desarrollar un programa que permita clasificar una región de interés de una imagen, dada su textura

- Programar un método que permita convertir una imagen de un espacio de color en el espacio CIELab

```sh
  //lECUTA de imagen a ser comparada, imagen original del test
  img = imread(PATHS_Test[i]+pathImagenDirectorioActualString);
  //resize(img, img, cv::Size(), 0.75, 0.75);
  //Pasamos la imagen al espacio de color CIELAB
  cvtColor(img, img, COLOR_RGB2Lab);
```
- Método programado que dada una imagen o región de interés permita calcular el descriptor LBP

```sh
   int* LBPDescriptor::LBP8(const int* data, int rows, int columns){
      const int
      *p0 = data,
      *p1 = p0 + 1,
      *p2 = p1 + 1,
      *p3 = p2 + columns,
      *p4 = p3 + columns,
      *p5 = p4 - 1,
      *p6 = p5 - 1,
      *p7 = p6 - columns,
      *center = p7 + 1;
      int r,c,cntr;
      unsigned int value;
      int* result = (int*) malloc(256*sizeof(int));
      memset(result, 0, 256*sizeof(int));
      for (r=0;r<rows-2;r++){
          for (c=0;c<columns-2;c++){
              value = 0;
              cntr = *center - 1;
              compab_mask_inc(p0,0);
              compab_mask_inc(p1,1);
              compab_mask_inc(p2,2);
              compab_mask_inc(p3,3);
              compab_mask_inc(p4,4);
              compab_mask_inc(p5,5);
              compab_mask_inc(p6,6);
              compab_mask_inc(p7,7);
              center++;
              result[value]++;
          }
          p0 += 2;
          p1 += 2;
          p2 += 2;
          p3 += 2;
          p4 += 2;
          p5 += 2;
          p6 += 2;
          p7 += 2;
          center += 2;
      }

      return result;
   }
   vector<int> LBPDescriptor::lbp(Mat img){
      vector<int> descriptor;

      int *data = (int *) malloc(img.rows*img.cols*sizeof(int));

      for(int i=0,k = 0;i<img.rows;i++){
          for(int j=0;j<img.cols;j++){
              //data[k]=img.at<uchar>(i,j);
              *(data+k) = img.at<uchar>(i,j);
              k++;
          }
      }

      int *desc = this->LBP8(data, img.rows, img.cols);
      for(int i=0;i<256;i++){
          descriptor.push_back(desc[i]);
      }

      free(data);


      return descriptor;
  }

  //Obtencion de matriz LBP de la imagen original
  descriptorLBP_imgOriginal = lbpd->lbp(imgGRAY);
```
- Determinar qué posturas estan clasificas correcta o incorrectamente
```sh
  def presicionDelModelo(original, prediccion):
    # Funcion para contar la cantidad de aciertos y fallos del clasificador #
    cont_aciertos = 0; 
    cont_fallos = 0;
    # Recorrido de valores originales #
    for i in range(len(original)):
        if (original[i] == prediccion[i]):
            cont_aciertos = cont_aciertos+1
        else:
            cont_fallos = cont_fallos + 1
    
    return cont_aciertos, cont_fallos
```
- Generación de gráfica para indicar la presición del descriptor de Zernike 
```sh
  def generarGraficaDePrecision(valores_reales, valores_prediccion_aciertos, valores_prediccion_fallos, directorios):
    #Obtenemos la posicion de cada etiqueta en el eje de X
    x = np.arange(len(directorios))
    #tamaño de cada barra
    width = 0.30

    #Generamos las barras para el conjunto de personas acostadas
    rects1 = ax.bar(x - width/2, valores_reales, width, label='Original')
    #Generamos las barras para el conjunto de personas de pie
    rects2 = ax.bar(x + width/2, valores_prediccion_aciertos, width, label='Prediccion - Aciertos')
    #Generamos las barras para el conjunto de personas de sentada
    rects3 = ax.bar(x + width/2+ width, valores_prediccion_fallos, width, label='Prediccion - Fallos')

    #Añadimos las etiquetas de identificacion de valores en el grafico
    ax.set_ylabel('Numero de imagenes por directorio')
    ax.set_title('Prediccion de imagenes con Zernike')
    ax.set_xticks(x)
    ax.set_xticklabels(directorios)
    #Añadimos un legen() esto permite mmostrar con colores a que pertence cada valor.
    ax.legend()

    #Añadimos las etiquetas para cada barra
    autolabel(rects1)
    autolabel(rects2)
    autolabel(rects3)
    fig.tight_layout()
    plt.savefig('graficaPresicionValores.png')
    #Mostramos la grafica con el metodo show()
    plt.show()
```
- Generacion de medidas de calidad y matriz de confusión
```sh
  def medidasDeCalidad(prediccion, salida):
    print (confusion_matrix(prediccion, salida)) 
    accuracy = accuracy_score(prediccion, salida)
    f1 = f1_score(prediccion, salida, average='weighted')
    recall = recall_score(prediccion, salida, average='weighted')
    precision = precision_score(prediccion, salida, average='weighted')
    print (classification_report(prediccion, salida))
```
## Resultados
### Matriz de Confusión
```sh
[[19  5  5]
 [ 9 20 11]
 [ 1  9  7]]
```
### Medidas de Calidad
 - Accuracy: 0.5348837209302325
 - F1 score: 0.5415304839723444
 - Recall: 0.5348837209302325
 - Precision: 0.5546898233509785
### Reporte de Clasificación
```sh
              precision    recall  f1-score   support

           0       0.66      0.66      0.66        29
           1       0.59      0.50      0.54        40
           2       0.30      0.41      0.35        17

    accuracy                           0.53        86
   macro avg       0.52      0.52      0.52        86
weighted avg       0.55      0.53      0.54        86
```
### Generación de gráfica para indicar la presición del descriptor de Zernike 
![](graficaPresicionValores.png)
