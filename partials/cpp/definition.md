# Examen 02 - Microcontroladores

## Planteamiento
Usando sensores de presión MPX4115 diseñados para medir la presion atomosférica entre 11 y 15 kPa con variación de 4.5MV cada 0.1kPa, requieren conexiones:

> **Pines:**
> 1. V_salida
> 2. GND
> 3. GND
> 4. 5.6. No conectados.

### Instrucciones
Se han de conectar cuatro de estos sensores, leídos por conversor Análogo-Digital.
Configurar pantalla LCD donde se muestra el promedio de la presión medida por los cuatro sensores en kPa.

Debe haber un pulsador conectado a una interrupción externa del microcontrolador, al presionarse se muestra por un período de tiempo de $T$ segundos, el mismo valor de presión pero en PSI. La base de tiempo es $b=0.5x$ segundos, con $x$ como último número de su código. El valor de $T$ está calculado mediante $T=Nb$, con $N$ como un valor que ingresa el usuario por el puerto serial cada que solicite ver el valor de presión en PSI.

### Consideraciones
- Un (1) kPa equivale a 0.145 PSI.
- **No olvidar que el timer se hace con un temporizador, nada de retardos.**
- **El plan es configurar el el conversor análogo/digital, nada de funciones que ya incluya arduino, sólo tipos/registros.**
- **Tampoco se permite librerías externas para usar los periféricos.**
- **Todo lo que se haga está diseñado para hacerse manualmente, igualmente las interrupciones.**
