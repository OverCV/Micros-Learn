# Microcontrollers Learning

## Descripción :pencil2: :books:

Este repositorio contiene información sobre el aprendizaje adquirido en dos microcontroladores de la familia Atmel, el ATmega328P y **ATmega2560**. Principalmente se a utilizado para el período lectivo 2024A el ATMega2560, de forma que hay mejor código en `.asm` y `.cpp` para este microcontrolador así como sus simulaciones en Proteus (`.pdsprj`).

Para facilitar el aprendizaje se tiene una carpeta `assets/` con imágenes de los PIN-OUT de cada microcontrolador, así como su **set de instrucciones** para conocer los mnemónicos utilizados en según rango de memoria y registros, así como su **datasheet** para conocer características técnicas como particiones de memoria, registros, uso y configuración de interrupciones, temporizadores, etc.

## Introducción a los microcontroladores :computer: :electric_plug: :bulb:

Un microcontrolador es un circuito integrado programable, capaz de ejecutar las órdenes grabadas en su memoria. Está compuesto por una unidad central de procesamiento, memoria y periféricos de entrada/salida. Los microcontroladores son utilizados en sistemas embebidos, es decir, sistemas electrónicos que realizan una función específica y que no son programables por el usuario.


### Instalación AVR Assembly :wrench: :hammer:

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