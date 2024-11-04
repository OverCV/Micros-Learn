
/*
 * 1-any.cpp
 * Created: 2024-10-27 00:10:33
 * Author: OverCV
 */

#include <avr/iomxx0_1.h>
#include <avr/iom2560.h>
#include <util/delay.h>
// importación de librerías sei e ISR
#include <avr/interrupt.h>
// #include<pins_arduino.h>
// #include<wirin>
// #include<wiring_digital.h>
// #include<arduino.h>

void setup() {
  // Configuración del ADC
  ADMUX = (1 << REFS0) | ADC; // Ref. AVcc y canal ADC0
  ADCSRA = (1 << ADEN) | (1 << ADPS2) |
           (1 << ADPS1); // Habilitar ADC con preescalador de 64

  // Configuración del temporizador 1 para PWM en modo Fast PWM 8-bit
  TCCR1A = (1 << WGM10) | (1 << COM1A1); // Modo PWM rápido, canal OC1A
  TCCR1B = (1 << WGM12) | (1 << CS11);   // Preescalador de 8

  pinMode(13, OUTPUT);

  // DDRB |= (1 << PW); // Configura PB como salida para OC1A (PWM)
}

void loop() {
  // Inicia una conversión ADC
  ADCSRA |= (1 << ADSC); // Inicia la conversión ADC
  while (ADCSRA & (1 << ADSC))
    ; // Espera a que termine la conversión

  int analogValue = ADC; // Lee el valor del ADC (0-1023)

  // Mapear el valor de ADC al rango PWM (0-255)
  int pwmValue = (analogValue * 255) / 1023;

  OCR1A = pwmValue; // Ajusta el ancho de pulso PWM
}

// class LED {
// public:
//   LED() { DDRB |= (1 << PB5); }
//   void toggle() { PORTB ^= (1 << PB5); }
// };

// int main(void) {
//   LED led;
//   /* Código aquí 🤗 */
//   while (1) {
//     led.toggle();
//     _delay_ms(10000);
//   }
//   return 0;

//   volatile int holi = 1;
// }

/*
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
2.5V  00000 11111 (511)
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


AnalogWrite(): Pasa un valor digital y lo vuelve análogo para escribirlo en un
motor. Si se activan los conversores y se necesitan los conversores de entrada o
salida según se necesite.
*/

/*
Usar el conversor análogo digital con una señal PWM, con un temporizador el
objetivo es que con una señal de un potenciómetro (res variable) conectado a 5V,
conectado se puede medir el ancho del pulso, si leen 5V pues el ancho es de 5,
si estamos en 2.5 la señal.
*/
