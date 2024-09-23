.include "m2560def.inc"

.org 0x0000
    RJMP main

.org 0x00AC
table:
    ; .dw 0xF9C0, 0xB0A4, 0x82F8, 0x9299, 0x9080
    .dw 0xF9C0, 0xB0A4, 0x82F8, 0x9299, 0x9080
    .dw 0x9890, 0x9218, 0xF880, 0x9090, 0x9088


main:
    ; Configurar el puerto A como salida (para el display de 7 segmentos)
    LDI     R16, 0xFF
    OUT     DDRA, R16

    ; Inicializar contador para recorrer los dígitos
    LDI     R18, 0        ; Usar R18 como contador de dígitos

loop:
    ; Establecer el puntero Z en la dirección de la tabla
    LDI     ZH, HIGH(table<<1)   ; Parte alta de la tabla flash
    LDI     ZL, LOW(table<<1)    ; Parte baja de la tabla flash

    ; Multiplicar el índice por 2 para acceder correctamente a cada dígito
    MOV     R19, R18
    LSL     R19                  ; Multiplicar el contador por 2 (índice * 2)
    ADD     ZL, R19              ; Ajustar puntero Z en consecuencia
    ADC     ZH, R1               ; Ajustar ZH si hay acarreo (R1 se asume como 0)

    ; Mostrar el dígito
    LPM     R16, Z      ; Leer el byte bajo
    OUT     PORTA, R16  ; Mostrar el dígito
    RCALL   delay

    ; Incrementar contador de dígitos
    INC     R18
    CPI     R18, 0             ; Comparar si ya hemos mostrado los 10 dígitos
    BRNE    loop                ; Si no hemos terminado, continuar el loop

    ; Reiniciar el contador cuando todos los dígitos se han mostrado
    LDI     R18, 0
    RJMP    loop

; Subrutina de retardo
delay:
    ldi  r20, 41
    ldi  r21, 150
    ldi  r22, 125
L1: dec  r22
    brne L1
    dec  r21
    brne L1
    dec  r20
    brne L1
    rjmp PC+1
    RET

;     .include "m2560def.inc"

;     .org(0x0000)
;     RJMP main

;     .org(0x0100)

; main:
;     ; Configurar el puerto A como salida (para el display de 7 segmentos)
;     LDI     R17, 0xFF
;     OUT     DDRA, R17

;     ; Inicializar contador para recorrer los dígitos
;     LDI     R18, 0        ; Usar R18 como contador de dígitos

; loop:
;     ; Establecer el puntero Z en la dirección de la tabla
;     LDI     ZH, HIGH(table<<1)   ; Parte alta de la tabla (tabla está en flash)
;     LDI     ZL, LOW(table<<1)    ; Parte baja de la tabla

;     ; Multiplicar el índice por 2 para acceder correctamente a cada par de dígitos
;     LSL     R18                  ; Multiplicar el contador por 2 (índice * 2)
;     ADD     ZL, R18              ; Ajustar puntero Z en consecuencia
;     BRCC    no_carry             ; Verificar si hay acarreo en ZL
;     INC     ZH                   ; Si hay acarreo, incrementar ZH

; no_carry:
;     ; Mostrar el primer y segundo dígito de la palabra
;     RCALL   show_digit
;     RCALL   delay

;     ; Incrementar contador de dígitos
;     INC     R18
;     CPI     R18, 10              ; Comparar si ya hemos mostrado los 10 dígitos
;     BRNE    loop                 ; Si no hemos terminado, continuar el loop

;     ; Reiniciar el contador cuando todos los dígitos se han mostrado
;     LDI     R18, 0
;     RJMP    loop

; ; Subrutina para mostrar los dígitos desde la tabla
; show_digit:
;     LPM     R16, Z+     ; Leer el primer byte (primer dígito)
;     OUT     PORTA, R16  ; Mostrar el primer dígito
;     RCALL   delay

;     LPM     R16, Z+     ; Leer el segundo byte (segundo dígito)
;     OUT     PORTA, R16  ; Mostrar el segundo dígito
;     RCALL   delay
;     RET

; ; Tabla de dígitos almacenada en la memoria de programa (flash)
; table:
;     ; Almacena dos dígitos por palabra
;     .dw 0xF9C0, 0xB0A4, 0xF882, 0x9299, 0x9080 ; 10 32 76 54 98

; ; Subrutina de retardo de 1 segundo
; delay:
;     ldi  r20, 41
;     ldi  r21, 150
;     ldi  r22, 125
; L1: dec  r22
;     brne L1
;     dec  r21
;     brne L1
;     dec  r20
;     brne L1
;     rjmp PC+1
;     RET
