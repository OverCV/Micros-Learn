    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

    .org(INT0addr)  ; 0x0002
    RJMP    isr_00     ; interruption service routine ;

/*
Definition: A program that shows an increasing or decreasing sequence whenever the user activates the INT0 interuption.
The interupt works that in every the button is pushed we're generating a level change.
___|---|___
*/

; def for renamings ;
.def seq    = R20
; 0: Increasing. 1: Decreasing. ;
.def state  = R21

main:
    ; Use the INT0 interruptions
    ; [0000.0001]
    LDI     R16,    1
    OUT     EIMSK,  R16
    ; [00 00 00 00]
    LDI     R16,    0
    STS     EICRA,  R16
    ; Set External Interruptions
    SEI
    ; Show the output (sequence)
    LDI     R16,    0xFF
    OUT     DDRC,   R16
    ; delay settings
    LDI     R27,    41
    LDI     R28,    150
    LDI     R29,    125
    LDI     R16,    1
    ; display and vector state initialization
    LDI     seq,    1
    LDI     state,  0

loop:
    ; vector de estado = [0000.000X] con X={0, 1}
    OUT     PORTC,  seq
    RCALL   half_sec_delay

    /*
    if state[0] == 0:
        seq++
    elif state[0] == 1:
        seq--
    */

    ; si el bit 0 de `state` vale 0 incrementa
    SBRS    state,  0   ; Skip if Bit Regisster Set (1)
    INC     seq
    ; si el bit 0 de `state` vale 1 decrementa
    SBRC    state,  0   ; Skip if Bit Regisster Clear (0)
    DEC     seq

    RJMP    loop

half_sec_delay:
    DEC     R27
    BRNE    half_sec_delay
    LDI     R27,    41
    DEC     R28
    BRNE    half_sec_delay
    LDI     R28,    150
    DEC     R29
    BRNE    half_sec_delay
    LDI     R29,    125
    RET

isr_00:
    ; [ 0000.000 x ] -> [ 0000.000 (not x) ]
    EOR     state,  R16
    RETI
