#!/usr/bin/env python
# coding: utf-8

# # Trabajo en Casa

# ### Realizado por: Juan Barrera y Katherine Barrera
# ---------------------------------------

## Importacion de librerias necesarias
import numpy as np
import os
from tqdm import tqdm
import time
import matplotlib.pyplot as pp
import mahotas
from math import sqrt
import pandas as pd
import collections
from sklearn.metrics import confusion_matrix  
from sklearn.metrics import accuracy_score
import matplotlib.pyplot as plt
from sklearn.metrics import f1_score
from sklearn.metrics import recall_score
from sklearn.metrics import precision_score
from sklearn.metrics import classification_report
import seaborn as sns

# Metodo que nos sirve para calcular la distancia euclidea sin libreria#
def calculoDistanciaEuclidea(momentosImgOriginal, momentosImgAComparar):
    # definicion de variable para almacenar resultado de la resta de los valores de cada momento #
    resta = 0.0
    # Recorrido de los momentos de nuestras imagenes #
    for i in range(len(momentosImgOriginal)):
        resta=resta+pow(momentosImgOriginal[i]- momentosImgAComparar[i],2);
    # Retorno de la distancia obtenida entre las imagenes #
    return sqrt(resta);


def obtencionMomentosZernike(image):
    # Calculamos los momentos de zernike con mahotas paso de un canal en especifico en este  #
    # caso seria el canal rojo y pasamos el tamano de la imagen que son de 512 #
    return mahotas.features.zernike_moments(image[:, :, 0], 512)


def leerImagen(pathImg):
    # Lectura de imagen con mahotas #
    img = mahotas.imread(pathImg)
    return img;

# Este metodo nos ayuda a comparar una imagene con las imagenes dentro de un directorio #
# Recibimos el directorio de imagenes con el que se comparara la imagen original #
# Recibimos el listado de imagenes del directorio con el que se va a comparar la imagen original #
# Recibimos los momentos de zernike de la imagen original #
def leerImagenesDeUnDirectorio(directorioActual, listImagenes, momentosImgOriginal):
    # Definimos una variable para almacenar la distancia euclidea con un valor alto #
    valorDistanciaEuclidea = 100
    # Recorrido del listado de imagenes #
    for i in tqdm(range(len(listImagenes))):
        # concatenacion de path para la lectura de cada imagen #
        pathImg = dirpath+directorioActual+'/'+listImagenes[i]
        if 'jpg' in pathImg:
            ##Lectura de la Imagen
            img = leerImagen(pathImg)
            # Obtencion de los momentos de zernike de la imagen recupeara anteriormente #
            momentosImgAComparar = obtencionMomentosZernike(img)
            # creamos una varibale auxiliar para almacenar el valor de la distancia #
            aux = valorDistanciaEuclidea
            # Llamada al metodo para el calculo de la distancia euclidea pasando #
            # de la imagen original y la imagen recien recuperada #
            valorDistanciaEuclidea = calculoDistanciaEuclidea(momentosImgOriginal, momentosImgAComparar)
            # si el aux es mayor que distancia euclidea recien calculada se cambia el valor del aux #
            # de lo contrario se mantiene el mismo valor sin modificar #
            if aux > valorDistanciaEuclidea :
                aux = valorDistanciaEuclidea
    
    return aux


    
def autolabel(rects):
        # Funcion para agregar una etiqueta con el valor en cada barra #
        for rect in rects:
            height = rect.get_height()
            ax.annotate('{}'.format(height),
                        xy=(rect.get_x() + rect.get_width() / 2, height),
                        xytext=(0, 3),  # 3 points vertical offset
                        textcoords="offset points",
                        ha='center', va='bottom') 

def cargaDataset(path):
    # Funcion para cada de dataset #
    df = pd.read_csv(path, low_memory=False, encoding= 'unicode_escape')
    # Modificacion del dataset a valores numericos #
    df['prediccion'] = df['prediccion'].replace({"acostado":0,"De-Pie":1,"Sentado":2})
    df['salida'] = df['salida'].replace({"acostado":0,"De-Pie":1,"Sentado":2})
    #print(df.head(10))    
    return df

def medidasDeCalidad(prediccion, salida):
    print("<<<<<<<<<<<<<<<<<<<<<Matriz de Confusion>>>>>>>>>>>>>>>.")
    print (confusion_matrix(prediccion, salida)) 
    print("<<<<<<<<<<<<<<<<<<<<<Medidas de Calidad>>>>>>>>>>>>>>>>.")
    accuracy = accuracy_score(prediccion, salida)
    f1 = f1_score(prediccion, salida, average='weighted')
    recall = recall_score(prediccion, salida, average='weighted')
    precision = precision_score(prediccion, salida, average='weighted')
    
    print ('Accuracy:', accuracy)
    print ('F1 score:',f1)
    print ('Recall:',recall)
    print ('Precision:',precision)
    print("<<<<<<<<<<<<<<<<<<<<<Reporte de Clasificacion>>>>>>>>>>>>>>>>.")
    print (classification_report(prediccion, salida))

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

if  __name__ ==  '__main__':
    
    # Creamos un dataset vacio con sus columnas para almacenar los resultados #
    df = pd.DataFrame(columns=['path','acostado', 'De-Pie', 'Sentado', 'prediccion','salida'])
    # carpeta en donde se encuntran las imagenes #
    dirpath = 'Corpus-Siluetas/'
    # direcotorios que contienen las imagenes #
    directorios = ['acostado', 'De-Pie', 'Sentado']
    # Recorrido de directorios para obtener las imagenes de cada uno de ellos #
    for directorio in directorios: 
        listaImagenesLeidas = os.listdir(dirpath+directorio)
        print("<<<<Cantidad de imagenes en directorio: ",directorio," = ", len(listaImagenesLeidas),">>>>")
        for i in tqdm(range(len(listaImagenesLeidas))):
            path = dirpath+directorio+'/'+listaImagenesLeidas[i]

            if 'jpg' in path:
                # Lectura de la Imagen #
                imgOriginal = leerImagen(path)
                # Obtencion de los momentos de zernike de la imagen recupeara anteriormente #
                momentosImgOriginal = obtencionMomentosZernike(imgOriginal)
                # Creacion de diccionario vacio para almacenar resultados de comparacion#
                valorComparacionDistancia = {}
                # Bucle para la comparacion de una imagen con todas las imagenes de cada uno de los directorios obtenidos #
                for directorioParaComparacion in directorios: 
                    # Lectura de imagenes para comparacion #
                    listaImagenesLeidasParaComparar = os.listdir(dirpath+directorioParaComparacion)
                    # Almacenamos el resultado de la distancia euclidea de cada directorio #
                    valorImagenMasParecida = leerImagenesDeUnDirectorio(directorioParaComparacion, listaImagenesLeidasParaComparar, momentosImgOriginal)
                    # Almacenamos en el diccionario la clave = nombre del directorio y valor = distancia euclidea mas pequena #
                    valorComparacionDistancia[directorioParaComparacion] = valorImagenMasParecida
                #  Obtencion del valor mas pequeno dentro del diccionario y ese es nuestra prediccion #
                minimo = min(valorComparacionDistancia, key=valorComparacionDistancia.get)
                # Agregamos los resultados obtenidos dentro del dataset #
                df = df.append({'path': path, 'acostado':valorComparacionDistancia['acostado']
                                , 'De-Pie': valorComparacionDistancia['De-Pie']
                                , 'Sentado': valorComparacionDistancia['Sentado']
                                , 'prediccion':minimo, 'salida': directorio}, ignore_index=True)      
            
    print("<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>")
    # Guardamos el resultado en archivo .csv #
    df.to_csv("ResultadosObtenidos.csv", index = False)

    df = cargaDataset('ResultadosObtenidos.csv')
    # Obtenemos los valores reales que deberiamos obtener el algoritmo #
    uniqueOriginal, countsOriginal = np.unique(df['salida'], return_counts=True)
    print("Resultados Verdaderos: ", dict(zip(uniqueOriginal, countsOriginal)))
    
    # Obtencion de las medidas de calidad de nuestro algoritmo #
    medidasDeCalidad(df['prediccion'], df['salida'])
    
    # Split de dataset para cada una de las salidas 0 = acostado, 1 = de pie, 2 = sentado #
    df_valoresSalida_Acostado = df.loc[df['salida'] == 0]
    # Obtencion de aciertos y fallos del algoritmo #
    aciertos_acostado, fallos_acostado = presicionDelModelo(df_valoresSalida_Acostado['prediccion'].tolist()
                                                        , df_valoresSalida_Acostado['salida'].tolist())
    
    
    df_valoresSalida_De_Pie = df.loc[df['salida'] == 1]
    aciertos_de_pie, fallos_de_pie = presicionDelModelo(df_valoresSalida_De_Pie['prediccion'].tolist()
                                                        , df_valoresSalida_De_Pie['salida'].tolist())

    df_valoresSalida_Sentado = df.loc[df['salida'] == 2]
    aciertos_Sentado, fallos_Sentado = presicionDelModelo(df_valoresSalida_Sentado['prediccion'].tolist()
                                                        , df_valoresSalida_Sentado['salida'].tolist())
    
    # Valores reales del conjunto de imagenes #
    valores_reales = [29,34,23]
    # Creacion de listado de aciertos de cada directorio #
    valores_prediccion_aciertos = [aciertos_acostado,aciertos_de_pie, aciertos_Sentado]
    # Creacion de listado de errores de cada directorio #
    valores_prediccion_fallos = [fallos_acostado, fallos_de_pie, fallos_Sentado]
    fig, ax = plt.subplots()
    # Generacion de grafica de aciertos y fallos #
    generarGraficaDePrecision(valores_reales, valores_prediccion_aciertos, valores_prediccion_fallos, directorios)
