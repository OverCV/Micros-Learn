# Inicio programaación Arduino-IDE

## Primera sesión

AnalogRead(): Para leer pines análoga, todos se pueden digital pero hay otros de
forma análoga, necesita recibir la información en digital pero si quiere
leer 3.3V y no lo interprete como 1 o 0 convierte esto a un número
representativo, por lo que.

En caso de 8 bits
5V:   1111 1111 (255)
2.5V  0000 1111 (127)
  0V  0000 0000 (0) ->  (0.019)  0000 0001

En caso de 10 bits
5V:   11111 11111 (1023)
2.5V  01111 11111 (511)
  0V  0000 0000 (0) ->  (0.0048V)  0000 0001

Entonces ya necesitamos estas conversiones de binario y a mayor cantidad pues
más resolucion tenemos que convertir de análogo a digital o viceversa.

Ya con ese dato en el registro es que se hacen cálculos, digamos si se lee un
termistor de forma que da .1V por cada grado, él entrega el voltaje a binario y
entonces podemos hacer el control de temperatura, de forma que si es mayor o
menor enciende o apaga el ventilador. Lee, convierte y actúa.

Si digamos un motor se le envía 0V nada, a la mitad pues la mitad de velocidad,
luego 5V pues la máxima velocidad, entonces se envía la señal de control y se
convierte a binario y se envía al motor.

Hay unos microcontroladores que pueden que tengan un pin para hacer una cosa, un
pin específico, o leer otro específico y convertirlo de forma que es un proceso
multiplexado en el tiempo, que escoge la señal entrante para el conversor y hace
parte de lo que se configura (como el timer), hay registros que toca definirles
el "canal de conversión (0->7)" para convertir la señal análoga.

### Primer ejercicio

AnalogWrite(): Pasa un valor digital y lo vuelve análogo para escribirlo en un
motor. Si se activan los conversores y se necesitan los conversores de entrada o
salida según se necesite.
