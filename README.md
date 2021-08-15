# Tratamiento Digital de imagenes: deteccion de bordes
## Entorno de trabajo
- Lenguaje C++
- Libreria IMtdi: creditos al Profesor D. Francisco Guindos Rojas
- Imagenes de entrada representadas en niveles de gris en formato bitmap
## Introducción
El enlazado de bordes es tras las técnicas mas importante en el Tratamiento Digital de Imágenes. En esta
práctica quiero mostrar una técnica mas sencilla basada en la convolución de la imagen inicial con una
máscara rápida y dos técnicas mas complejas basadas en el análisis de la 8-vecinidad del pixel céntrale.
Los bordes son una información importante de la imagen, enseñan la posición de los objectos, el tamaño la
forma, etc. Un borde es un área donde el nivel de gris cambia da un valor bajo a un valor alto o contrario. El
borde el exactamente el centro de esta transición.
La técnica mas popular para detectar bordes es la convolución de la imagen inicial con máscaras (casi
siempre 3x3). La idea es de hacer una multiplicación punto-punto tras la máscara y una sección (del mismo
tamaño de la máscara) de la imagen. Luego se suman todos los productos y el resultado será puesto en el
pixel central.
El numero de mascaras posibles es sin límites. Las más utilizadas son Kirsch, Prewitt, y Sobel. Este suele
tener buenos resultados, pero son mascaras direccionales (“compass gradient”). Por esto, pueden detectar
bordes en una sola dirección por cada operación de convolución; pues se necesita 8 convoluciones para
detectar bordes en la 8-vecinidad de un pixel.
Sin embargo, hay 2 principios básicos que deben respectar:
- La suma de los números de la máscara debe ser 0
- La mascara debe aproximar la operación de diferenciación o aumentar el pendiente del borde.
Mascara rápida
La primera técnica que he implementado usa una máscara rápida (“quick mask”), que puede detectar
bordes en todas las direcciones con una sola convolución. La ventaja principal es claramente una mayor
velocidad de cómputo, sin embargo, hay situaciones donde se necesita detectar bordes en una dirección
particular y esta mascara no va a ser adecuada.
Después de la operación de convolución es buena norma umbralizar la imagen para que tenga valores
entre 0-255 y también para quitar parte del ruido y bordes débil. 

## Homogenity operator
La técnica que vamos a ver ahora no utiliza la convolución, la idea de esta técnica es la resta para detectar
un borde. El operador substrae cada pixel que rodea al pixel céntrale en una 8-vecinidad.
El resultado es el máximo en valor absoluto de estas restas. Si el área es uniforme la resta será cero y
quiere decir que no hay bordes. 
La funcion homogenity() implementa este operador. Tiene un parámetro de entrada, la imagen original, y
un parámetro de salida, la imagen de output. En el bucle externo se itera la imagen desde la segunda línea
y secunda columna y se pone la diferencia máxima a cero.
En el bucle interno se hace la resta tras el pixel céntrale y cada pixel de la máscara. Luego se calcula el valor
absoluto de estas operaciones y si es mayor que el máximo valor se actualiza el máximo y al final del bucle
se pone el mismo valor en la imagen de salida.
Después hay la posibilidad de umbralizar la imagen, como vamos a ver después en las pruebas, sin
umbralizar la imagen de salida habrá bordes difuminados.
En las pruebas que he hecho, un valor de umbral cerca de 25 da bueno resultados en general. 

## Contrast Edge detector
Un problema con el enlazado de bordes es la luz no uniforme. El operador que vamos a ver ayuda a reducir
este problema. Este detector se usa el resultado de cualquier operador de enlazado de bordes y divide los
valores por el valor medio de la vecindad. La división quita el efecto de luz no uniforme y el valor medio se
calcula con la convolución de una máscara que tiene todos los elementos igual a 1 y luego dividendo para el
tamaño del área. Es importante hacer la distinción tras bordes en áreas bien iluminada y poco iluminada. 

## Conclusiones
Como se puede ver en las pruebas, la técnica de la máscara rápida da buenos resultados sobre todos en los
test 1-2 donde no hay mucho ruido y los detalles finos se guardan en la imagen de salida. Sin embargo,
como ensena el test 4, si hay una iluminación y un fondo no uniforme, la imagen de salida habrá ruido y
bordes discontinuos.
El operador de homogeneidad resulta ser el mejor, los bordes son mas coherentes y fuerte, además si se
aplica el umbral se quita el efecto de difuminación de los bordes (y un poco de ruido). El inconveniente es
que como se puede ver en el test 2 se quita una parte de los objectos pequeños (bordes de la gota de
agua).
El operador de detección de borde con contraste en teoría habría tenido que ser el mejor, especialmente
en el test 4, todavía siempre ha dado resultados insuficientes. Una futura implementación podría una
mascara de bordes mas suave, para guardar bordes mucho débiles, y también otras imágenes más
adecuadas.
