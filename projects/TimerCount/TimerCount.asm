    .include"m328Pdef.inc"

    .org(0x0000)
    RJMP    main

    .org(0x0020)            ; TIMER0 OVF
    RJMP    rsi_0

    ; Diseño de programa cual hace cuenta de 8 bits por el puerto B.
    ; Incrementa mediante el interrupción del Timer0 (reloj interno + pre-escalamiento).
main:
    SEI                     ; Inicializar interrupciones

    LDI     R16,    0xFF
    LDI     R17,    0x80
    OUT     DDRD,   R16     ; PU - Salida

    ; Inicializar puntero de pila
    OUT     SPH,    R17
    OUT     SPL,    R16

    LDI     R18,    0x00    ; Mostrar valor inicial
    OUT     PORTD,  R18

    LDI     R19,    0x05    ; MAX Pre-Scaler (0b101) clkI/O/1024 (from prescaler)
    OUT     TCCR0B, R19     ; Pre-Scaler selection

    LDI     R20,    0x01    ; Habilita (0b1) interrupciones por desbordamiento
    STS     TIMSK0, R20     ; Timer/Counter Interrupt Mask Register

    LDI     R21,    0x00    ; Valor Contador (Cuenta interna del timer)
    OUT     TCNT0,  R21     ; Acá almacena la cuenta

    ; R21 cuenta los ciclos del oscilador, de forma que al desbordarse genera interrupción para incremento en contador

loop:
    RJMP    loop

rsi_0:
    INC     R18             ; Este conteo es distinto al de R21.
    OUT     PORTD,  R18     ; Cuenta externa (vía led)
    RETI