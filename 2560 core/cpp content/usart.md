# USART

La comunicación paralela sólo dice que los dispositivos tienen una comunicación serial o paralela.


La comunicación que es serial pues tenemos un cable para enviar los datos.
Con ello se necesita un tipo de reloj que le diga que puede leer un dato y el siguiente, de forma que si hay líneas de Tx y Rx porque es asíncronas, las síncronas s+í necesitan la línea de reloj. Un dispositivo tiene un trasnmisor que se conecta al receptor del otro, esa conexión requiere sólo essa sin importar el tamaño del dato. Entonces se requiere de protocolos, uno es el USART y el otro el I2C y pues el orden de las señales y así es el protocolo.

La USART permite tanto síncrona como asíncrona, ambos Tx y Rx tienen reloj pero, el punto es que son relojes independientes pero estos relojes tengan la misma velocidad. La velocidad de la USART es por baudios, cuando ponemos el 9600 por ejemplo en la terminal. Esta permite que una conmunicación sea Full-Duplex, es decor que se puede recibir y enviar al mismo tiempo, ambos pueden transmitir y recibir datos al mismo tiempo. La comunicación Half-DUplex es que hay un sólo canal así que si el canal está ocupado pues sólo puede uno.


Hay la opción de comunicación maestro-esclavo. Esta es para cuando hay un dispositivo (microproc) que se comunica con varios sensores, varios sensores que dan información, el maestro es el micro, los sensores o actuadores o lo que sea son esclavos, de forma que no es comunicación punto a punto sino de un punto a múltples, los esclavos tienn un nombre o dirección, el maestro dice que tiene que comunicarse con X periferico (su dirección) y el periferico le responde que si puede, y luego el maestro dice qué quiere y el periferico responde con la información que le pide.

El formato de la trama es simple, dice que la línea está en 1 si no hay comunicación, se da un bit de ST o start que es poner la línea en 0 e indica inicio de comunicación, luego están los bits del dato que son 4, y luego en corchetes están porque para este en particular es para decir si queremos un dato de 6, 7, 8, 9 bits, entonces si el dato llega al 5 y se pasan a los bits de P o paridad, si trabajamos con 9 entonces llegamos al 8 y sigue el de paridad. Este bit de paridad es lgo simple para detección de errores *(la paridad es que si uno trabaja con el bit de paridad tiene que haber una cantidad par de 1's y par de 0's, si hay un 1 y un 0 entonces hay un error, por lo que se hace un par de 1's y par de 0's)*.

Luego están los SP1 y SP2 y finalmente el ST/IDLE que es como estaba al origen.

Configurar:

1. Velocidad en baudios.
2. Formato de la trama (si el transmisor emite 6 bits pues el receptor también, si es paridad par el receptor también, etc.).
3. El inicio de la transmisión no se da si no se habilitan el Tx y Rx, el EN o enable, así mismo la USART, en estos debe estar habilitados.
4. Decidir si queremos usar interrupciones o no, si queremos INT o FLAG banders (que es con pooling) pero pues obviamente es con INT porque el Pooling es malísimo.

Luego se muestran las tablas que no coinciden porque son sacadas del Arduino, pero son las mismas ecuaciones (por eso no las cambió), el $BAUD = \frac{f_{OSC}}{16(UBRRn+1)}$ y $UBRRn = \frac{f_{OSC}}{16()}-1$.

Esta USART cuenta con tres banderas de error, la FE0 indica si hay error de trama (si sí se leyó bit de parada, porque si no llega es que no se envió), el otro es de sobreflujo de datos DOR0 porque el registro que se manda además está un buffre para guardar lo que se envió en la cola, pero sí se envía otro pues es porque el emisor está enviando muchas cosas, una frecuencia distinta.
Luego está la detección de errores así que no se suele usar (se configura pero no es necesaria del todo usarla).

Entonces en el registroe UCSRnA están las banderas de error, por lo que se puede leer con el bit de bandera de error de la bandera de error.

### Notas importantes:

- Si usamos comunicación de 9 bits es porque este bit está en otro registro de forma que el orden es importante, este 9no bit debió ser enviado o leído antes de poner los otros 8, si se hace al revés pues hay problemas.
- Si hacemos detección de erorres primero se leen las banderas y luego el dato porque sinó se pueden borrar, las de error NO son interrupciones, el terminar de emitir o transmitir sí son interrupciones, pero las de error no, no hay formita de ir a mirar.
- Si se inhabilita el transmisor envía y se muere, pero si se apaga el receptor pues se muere el mensaje.
- El buffer de transmisión sólo puede ser cargado si el bit UDRE0 es 1, o sea que está vacío.
- La transimsión siempre que el transmisor esté habilitado pues luego carga el buffer y entonces sí se puede empezar a transmitir.

### Registros

UCSRnA, en este están los habilitadores de EN tanto para Receptor como transmisor, los siguientes 3 son para, no sabe, el 4to es el de error de trama, sigue desbordamiento y paridad, el otro es habilitar transmisión a doble de velocidad 1 habilita, el último no se acuarda.
El UDREn es para decir si el buffer está vacío o no. El noveno bit es los que están con la recepción y transmisión. Mentiras, los primeros bits 7 y 6 son las banderas para hacer pooling (osea no).

En el UCSRnB los primeros 2 son para la habilitación de interrupciones de transmisión y recepción. El bit 4 es para la interrupción. Los bits . EL noeno bit de transmisión y recepción están en los indices 0 y 1.

Entonces en las hojas de datos, por ejepmlo el protocolo SPI es maestro-esclavo, los UPmnX es para la paridad
Los USBSn son para saber si usa un bit o dos de parada.

Los UCSZn, esos 3 son para definir con cuantos bit van a enviar datos.
El UCPOLn es para la polaridad de la señal de reloj en la comunicación síncrona. Como es síncrono requiere una línea de reloj, de forma que se configura la polaridad, osea en que flando de reloj se revisa si es con flanco de subida o bajada, ahí está esa configuración.
Esto está en la sección 22, la tabla de la parte de 16MHz, la tabla 22-12, miramos que si queremos trabajar con X baudios ponemos un valor u otro en los registros, pero si es con doble valor entonces pues configuramos una forma u otra.

### Ejercicio fácil de clase

Por hoy debemos poner a funcionar en modo asíncrono, hacemos la simulación en proteus con el virtual terminal que es el que se comunica, enviamos un dato y podemos imprimir algo del teclado y se mande. Como no hay otra forma de mostrar es oprimir una tecla, lo envia el virtual al micro y se manda de vuelta para que se muestre.
Entonces es que efectivamente se entienda que se recibe y se muestra. Es el teclado del computador. Sería una virtual terminal que, que se oprima la tecla y hagag eso.
