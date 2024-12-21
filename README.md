# Microcontrollers Learning

## Descripción :pencil2: :books:

Este repositorio contiene información sobre el aprendizaje adquirido en dos microcontroladores de la familia Atmel, el ATmega328P y **ATmega2560**. Principalmente se a utilizado para el período lectivo 2024A el ATMega2560, de forma que hay mejor código en `.asm` y `.cpp` para este microcontrolador así como sus simulaciones en Proteus (`.pdsprj`).

Para facilitar el aprendizaje se tiene una carpeta `assets/` con imágenes de los PIN-OUT de cada microcontrolador, así como su **set de instrucciones** para conocer los mnemónicos utilizados en según rango de memoria y registros, así como su **datasheet** para conocer características técnicas como particiones de memoria, registros, uso y configuración de interrupciones, temporizadores, etc.

## Introducción a los microcontroladores :computer: :electric_plug: :bulb:

Un microcontrolador es un circuito integrado programable, capaz de ejecutar las órdenes grabadas en su memoria. Está compuesto por una unidad central de procesamiento, memoria y periféricos de entrada/salida. Los microcontroladores son utilizados en sistemas embebidos, es decir, sistemas electrónicos que realizan una función específica y que no son programables por el usuario.


### Instalación AVR Assembly :wrench: :hammer:

AVR Support
[CTRL + Shift + P] Change Language Mode -> AVR Assembler (avr)
Mips assembler for formatting.
En el archivo `2560 core\.assets\tools\iom2560.h` se encuentran las declaraciones de registros, direcciones de memoria, interrupciones, timers, etc...

## Programación de alto nivel (Arduino/C/Cpp)

Voy a presentar dos alternativas en orden de facilidad para ejecutar los aplicativos necesarios para esta segunda sección de la programación en el ATMega2560.

Hay un malentendido en esta sección y es que, hay compatibilidad entre programar en un fichero `.ino`, uno de tipo `.cpp` y finalmente uno `.c`, de forma que lo únicamente relevante en la programación en arduino es que hagamos uso adecuado de las importaciones u definiciones de archivos de cabecera (`.h`) para cualquier lógica modularizada que vayamos a desarrollar.
De esta forma programar en arduino **no** es únicamente programar en C, ni en Cpp, ni es sólo Ino, es una programación en conjunto de estos lenguajes.

### Instalación de Arduino IDE

No sólo acá se explicará la instalación sino también el uso de este entorno pues puede causar problemas el no aplicar o conocer cómo hacer operaciones básicas.

Instalación en ...

Es importante entonces saber que al crear un archivo `.ino` se requiere que la carpeta que lo contenga tenga el mismo nombre, si no, no compilará hasta que se haga.
Para poder visualizar el directorio de nuestro archivo, pulsamos `[CTRL + Shift + E]` de forma que podemos visualizar todos los archivos e incluso la carpeta `build` de compilación.

Para compilar un fichero `.ino` pulsamos `[CTRL + Alt + S]`, con esto seleccionaremos la carpeta de compilación `build`, de la cuál únicamente nos interesa su fichero `.hex`, cual daremos clic derecho, `copy path` y en un simulador *(ex. Proteus 8)* con el arduino dispuesto, cliqueamos en el mismo y pegamos esta ruta del fichero hexadecimal *(nada de ir a buscarlo/navegar por directorios)*.


### Instalación C++ :wrench: :hammer:

Primero descargarse las herramientas desde [GCC Compiler](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers), luego el [AVR Dude](https://github.com/avrdudes/avrdude/releases) y Makefile, omitible si ya se tiene.

Luego debe añadirse el script python `avrdude.py` en las variables del sistema así mismo como las carpetas ya instaladas.
Se puede probar con 

```bash
avr-gcc --version
```

```bash
avrdude --version
```

```bash
make --version
```
Cambiar `C_Cpp.clang_format_style` con `{BasedOnStyle: llvm, BreakBeforeBraces: Attach}`