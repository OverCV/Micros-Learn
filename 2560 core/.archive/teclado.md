# teclado

Al oprimirse una tecla el cable de la columna se junta con el de la fila, por debajo hay un cable en la tecla, eso es todo lo que se hace.
Para saber cuál tecla se oprime hay que detectar la fila y columna juntada, hay que sacar un dato por fila y columna y leer las otras, de acuerdo a la lectura se sabe cuál se oprimií, esto es mediante la rotación es un 0 o un 1, entonces quedaría como

Si no se lee 0 en ningúna es porque ninguna se oprimió, entonces se pone el 0 en la siguiente fila, si lee un 0 se sabe cuál se oprimió, por lo que se tendrá la tecla presionada.

La rotación del 0 se hace con un tiempo ajustado porque no puede ser muy lenta porque si es muy lenta, entonces el 0 llega y no llega, pero entonces si es muy rápido entonces se oprime una vez y se detecta varias veces.

Otra cosa a considerar es que si hacemos la rutina de lectura en teclado, hay librerías y hay una para lectura de teclado matricial, pero "recuerden lo que nos dice que nos quedemos haciendo un pooling, y las líneas que se queden leyendo todo el tiempo, para eso están las interrupciiones por cambio y por eso son por grupos, tal que los 4 pines se conecten a una inter de un mismo grupo y cuando se genera entonces va a leer el dato, si hay algúno cambiado". Quiere ser específica y que las lecturas son con interrupciones, no con pooling que están en internet. Es un ejercicio para practivar las PCINT. En este caso como la lectura es del teclado pide que el numerito salga por el display 7 segmentos.