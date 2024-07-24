.include "m328Pdef.inc"

.org 0x0000
    rjmp main

; Ejecución de un semáforo
main:
    ; pull-up salidas
    LDI R16,  0xFF
    OUT DDRB, R16

    ; Inicializar puntero de pila en 16 bits
    LDI R17, 0x08
    OUT SPL, R16

    OUT SPH, R17

start:
    SBI PORTB, 2    ; Activar LED Verde
    RCALL Delay3     ; subrutina +3 segundos
    CBI   PORTB, 2  ; Desactivar LED Verde

    RCALL Blink
    RCALL Blink
    RCALL Blink

    SBI   PORTB, 1
    RCALL Delay3
    CBI   PORTB, 1  ; Activar LED Amarillo

    SBI   PORTB, 0  ; Desactivar LED Amarillo
    RCALL Delay3
    RCALL Delay3
    CBI   PORTB, 0  ; Activar LED Amarillo

    RJUMP loop

loop:

; main:
;     ; Código aquí 🤗
;     LDI R19,    0x00000000
;     LDI R20,    0x00100111
; loop:
;     INC  R19

;     BRBC 1, loop
;         INC R20
;     BRBC 1, loop