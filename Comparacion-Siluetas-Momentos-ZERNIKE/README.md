# Computer Vision para la identifiacion Momentos Zernike

1. Desarrollar un programa que permita calcular el Histograma de Gradientes Orientados (HOG) de
zonas de interés en una imagen a fin de detectar peatones, para ello deberá considerar los siguientes
aspectos

- Calculo los momentos de Zernike de todas las imágenes contenidas en el archivo “CorpusSiluetas.zip”
```sh
  pip install mahotas
  import mahotas
  def obtencionMomentosZernike(image):
    # Calculamos los momentos de zernike con mahotas paso de un canal en especifico en este  #
    # caso seria el canal rojo y pasamos el tamano de la imagen que son de 512 #
    return mahotas.features.zernike_moments(image[:, :, 0], 512)
```
- Medir la distancia Euclídea 
- Determinar qué posturas estan clasificas correcta o incorrectamente
- Generación de gráfica para indicar la presición del descriptor de Zernike 
![](images/image--003.png)
- Generacion de medidas de calidad y matriz de confusión
