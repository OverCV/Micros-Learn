    .include"m328Pdef.inc"
    .org(0x0000)
    ; RJMP    Inicio

    ldi     r16,        0x00
    out     DDRB,       r16

start:
    in      r16,        PIND    ; Leer los datos del puerto B
    mov     r17,        r16     ; Copiar los datos leídos a otro registro


    andi    r16,        0x0F    ; Quitar los bits no deseados
    andi    r17,        0xF0    ; Quitar los bits no deseados
    swap    r17

    in      r18,        PIND    ; Leer los bits de operación del puerto D
    andi    r18,        0x0c    ; Quitar los bits no deseados

    ; Realizar la operación según los bits de operación
    cpi     r18,        0x00    ; Comparar con 00 (suma)
    breq    suma
    cpi     r18,        0x40    ; Comparar con 01 (resta)
    breq    resta
    cpi     r18,        0x80    ; Comparar con 10 (AND)
    breq    andd
    cpi     r18,        0xc0    ; Comparar con 11 (OR)
    breq    orr

suma:
    add     r16,        r17     ; Sumar los valores de r16 y r17
    cpi     r16,        0x10
    brlt    mostrar
    rjmp    start

resta:
    mov     r20,        r17
    mov     r21,        r16


    sub     r17,        r16     ; Restar los valores de r17 y r16
    mov     r16,        r17
    cp      r20,        r21
    brge    mostrar
    ldi     r22,        0x00
    out     PORTB,      r22
    rjmp    start

andd:
    and     r16,        r17     ; Realizar la operación AND entre r16 y r17
    jmp     mostrar

orr:
    or      r16,        r17     ; Realizar la operación OR entre r16 y r17

mostrar:
    out     PORTB,      r16     ; Escribir el resultado en el puerto D
    rjmp    start               ; Volver al inicio del bucle




