    .include    "m2560def.inc"

    /*
    ; Primero - Led de carreras 🏁:
    ; En un display siete segmentos hacer que un único led esté siempre encendido y realice un circuito completo, se recibe una entrada de un bit para seleccionar entre:
    ; - En 0 el led hace un círculo en cualquier sentido (como el dígito 0).
    ; - En 1 el led hace un ocho.
    ; El led siempre vuelve a su posición original y repite la secuencia a no ser que el usuario pues cambie la entrada, el timer para cambiar es de medio segundo.

    ; Requisito:
    ; - Debe usarse un único puerto.

    ; Objetivo:
    ; - Familiarizarse con entradas, salidas y Display 7Segmentos y subrutina de timer.
    */

    .org        (0x0000)
    RJMP    main

    ; timer/counter compare match
    .org        (OC0Aaddr)
    RJMP    tim_isr_0


main:
    ; Código aquí 🤗

    LDI     R16,        0x7F    ; 0111 1111
    OUT     DDRA,       R16

    ; círculo
    LDI     R18,        0x01    ; 0000 0001

    ; Vector de estado
    LDI     R25,        0x00
    LDI     R26,        0x00

    SEI

timer_clock_conf:
    ; Configurar Timer0 en modo CTC (Clear Timer on Compare Match)
    LDI     R16,        0x02    ; WGM01 = 1, modo CTC
    OUT     TCCR0A,     R16
    ; Establecer el valor máximo en OCR0A
    LDI     R16,        160     ; AQUÍ (R, el máximo del registro)
    OUT     OCR0A,      R16     ; Valor de comparación en OCR0A
    ; Configurar el prescaler en 1024 (CS02 = 1, CS01 = 0, CS00 = 1)
    LDI     R16,        0x05
    OUT     TCCR0B,     R16
    ; Habilitar la interrupción de comparación para el Timer0 (OCIE0A)
    LDI     R16,        0x02    ; Habilitar interrupción de comparación (OCIE0A)
    STS     TIMSK0,     R16
    ; Inicializar el contador de interrupciones (K)
    LDI     R17,        49      ; AQUÍ
    LDI     R19,        0

loop:
    IN      R17,        PINA    ; X111 1111

    ANDI    R17,        0x80
    SBRS    R17,        7

    RJMP    circulo
    RJMP    ocho


circulo:
    ; Puerto A: Pines: 7 6 5 4 3 2 1 0 (0GFE DCBA)
    OUT     PORTA,      R18

    BRLO

    ; Comparación, al llegar a 6 setear R18 en 1

    LSL     R18

    LDI     R16,        0x01
    CPSE    R25,        R16     ; Skif if Bit Register is CLEAR
    ; LDI     p
    ; LDI


    ; Si no es mayor al valor actual no entra al cambio mediante la subrutina

    LDI     R18,        0x01

    CPI     R18,        0x00


    RJMP    loop

ocho:
    RJMP    loop

tim_isr_0:
    DEC     R17
    BRNE    tim_reti
    LDI     R17,        49

    ; AQUÍ LA LÓGICA

    INC     R25

    ; SBRC    R25,            0

    ; SBRS    R25,            0
    ; ADD     ZL,             R16

tim_reti:
    RETI

    /*

    ; V0 = [xxxx.xxxb] El que determina en qué secuencia estamos
    ;     b0 = 0: Hacemos el 0
    ;     b0 = 1: Hacemos el 8

    ; V1 = [xxxx.bbbb] El que detemrina la posición del led haciendo el 8
    ;     Conteo para determinar la posición del led

    ; [x, x]
    ; [x, x]

    */