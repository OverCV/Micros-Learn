
/*
 * 1-any.cpp
 * Created: 2024-10-27 00:10:33
 * Author: OverCV
 */

#include <avr/io.h>
#include <avr/iom2560.h>
#include <util/delay.h>

class LED {
public:
  LED() { DDRB |= (1 << PB5); }
  void toggle() { PORTB ^= (1 << PB5); }
};

int main(void) {
  LED led;
  /* Código aquí 🤗 */
  while (1) {
    led.toggle();
    _delay_ms(10000);
  }
  return 0;

  volatile int holi = 1;
}

/*
AnalogRead(): Para leer pines análoga, todos se pueden digital pero hay otros de forma análoga, necesita recibir la información en digital pero si quiere leer 3.3V y no lo interprete como 1 o 0 convierte esto a un número representativo, por lo que.

En caso de 8 bits
5V:   1111 1111 (255)
2.5V  0000 1111 (127)
  0V  0000 0000 (0) ->  (0.019)  0000 0001

En caso de 10 bits
5V:   11111 11111 (1023)
2.5V  00000 11111 (511)
  0V  0000 0000 (0) ->  (0.0048V)  0000 0001

Entonces ya necesitamos estas conversiones de binario y a mayor cantidad pues más resolucion tenemos que convertir de análogo a digital o viceversa.


Ya con ese dato en el registro es que se hacen cálculos, digamos si se lee un termistor de forma que da .1V por cada grado, él entrega el voltaje a binario y entonces podemos hacer el control de temperatura, de forma que si es mayor o menor enciende o apaga el ventilador. Lee, convierte y actúa.

Si digamos un motor se le envía 0V nada, a la mitad pues la mitad de velocidad, luego 5V pues la máxima velocidad, entonces se envía la señal de control y se convierte a binario y se envía al motor.

Hay unos microcontroladores que pueden que tengan un pin para hacer una cosa, un pin específico, o leer otro específico y convertirlo de forma que es un proceso multiplexado en el tiempo, que escoge la señal entrante para el conversor y hace parte de lo que se configura (como el timer), hay registros que toca definirles el "canal de conversión (0->7)" para convertir la señal análoga.


AnalogWrite(): Pasa un valor digital y lo vuelve análogo para escribirlo en un motor. Si se activan los conversores y se necesitan los conversores de entrada o salida según se necesite.


 */
