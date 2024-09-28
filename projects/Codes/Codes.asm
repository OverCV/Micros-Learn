    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

    ; En este programa se almacenará el código y cédula de un estudiante en la memoria Flash del programa y se mostrará por un conflay 7Segmentos ánodo común.
    ; Según una entrada de 02 bits se mostrará el código (0) o cédula (1) con el bit menos significativo, el más significativo mostrará los dígitos en orden ascendente (0) o descendente (1).

    ; 00: ASC Código. 01: ASC Cédula. 10: DESC Código. 11: DESC Cédula ;

    ; Se usarán los puertos B0 y B1 para posterior implementación con interrupciones.
main:
    ; Puerto B como entrada (bits 0, 1 do care)
    LDI     R16,            0xFC
    OUT     DDRB,           R16

    ; Puerto A como salida (bit 7: don't care)
    LDI     R16,            0x7F
    OUT     DDRA,           R16

    LDI     R21,            0x09

loop:
    IN      R16,            PINB
    ; Vamos al tipo de identificación
    SBRS    R16,            0
    ; Entra al primero (cod), sino al segundo (ced).
    RJMP    conf_cod
    RJMP    conf_cc

conf_cod:
    SBRS    R16,            1
    ; Si no vamos de modo ASC iremos DESC
    RJMP    reset_x_desc
    RJMP    reset_x_asc

    ; Leer posición y desplazar, si el bit 7 no es 0 reinicia, sino muestra por 7S ;

reset_x_asc:
    LDI     ZH,             0x00
    LDI     ZL,             0x60
    LSL     ZL
    ROL     ZH
cod_asc:
    ; Si estamos en X0 o 0X nos quedamos
    IN      R19,            PINB
    SBRC    R19,            0
    JMP     loop
    SBRC    R19,            1
    JMP     loop

    LPM     R20,            Z+
    SBRC    R20,            7
    RJMP    reset_x_asc

    OUT     PORTA,          R20
    RCALL   delay
    JMP     cod_asc


reset_x_desc:
    LDI     ZH,             0x00
    LDI     ZL,             0x60
    LSL     ZL
    ROL     ZH
    ADD     ZL,             R21
cod_desc:
    ; Si estamos en X0 o 1X nos quedamos
    IN      R19,            PINB
    ; SBRC    R19,            0
    ; JMP     loop
    ; SBRS    R19,            1
    ; JMP     loop

    LPM     R20,            Z
    DEC     R30
    ; SBRC    R20,            7
    CPI     ZL,            0x00
    RJMP    reset_x_desc

    OUT     PORTA,          R20
    RCALL   delay
    JMP     cod_desc

    ;!!;

conf_cc:
    CPI     R18,            0x00
    ; Si no vamos de modo ASC iremos DESC
    BREQ    reset_y_asc
    RJMP    reset_y_desc

reset_y_asc:
    LDI     ZH,             0x00
    LDI     ZL,             0x80
    LSL     ZL
    ROL     ZH
ced_asc:
    ; Si estamos en X1 o 0X nos quedamos
    IN      R19,            PORTB
    SBRS    R19,            0
    BRNE    loop

    LPM     R20,            Z+
    SBRC    R20,            7
    BREQ    reset_y_asc

    OUT     PORTA,          R20
    RCALL   delay
    RJMP    ced_asc

reset_y_desc:
    LDI     YH,             0x00
    LDI     YL,             0x80
    LSL     YL
    ROL     YH
    ADD     XL,             R21
ced_desc:
    ; Si estamos en X1 o 1X nos quedamos
    IN      R19,            PORTB
    SBRS    R19,            0
    JMP     loop

    LPM     R20,            Z;-
    SUBI    ZL, 1        ; Restar 1 a ZL (parte baja de Z)
    SBCI    ZH, 0        ; Restar con acarreo en ZH (parte alta de Z)
    SBRC    R20,            7

    BREQ    reset_y_desc

    OUT     PORTA,          R20
    RCALL   delay
    RJMP    ced_desc

delay:
    ; Subrutina de retraso (1 seg)
    LDI     R20,            50
    LDI     R21,            10
    LDI     R22,            10
L1:
    DEC     R22
    BRNE    L1
    DEC     R21
    BRNE    L1
    DEC     R20
    BRNE    L1
    RET

table_cod:
    ; Tabla con código estudiante
    .org(0x0060)
    .dw 0x4040, 0x4040, 0x2440, 0x3024, 0x0210  ;   cod (bit 7 = 0)
    ; .dw 0xC0C0, 0xC0C0, 0xA4C0, 0xB0A4, 0x8290  ;   cod (bit 7 = 1)

table_ced:
    ; Tabla con cédula estudiante
    .org(0x0080)
    .dw 0x4079, 0x2440, 0x1902, 0x4030, 0x2479  ;   ced (bit 7 = 0)
    ; .dw 0xC0F9, 0xA4C0, 0x9982, 0xC0B0, 0xA4F9  ;   ced (bit 7 = 1)



    ; Con interrupciones entonces:
    ; ciclo -> ciclo
    ; INT0: Set code in a var
    ; INT1: Set dir in a var
    ; down -> Exec code that vary by those conds

    ; Con la subrutina llevar a secciones donde el flujo normal impediría setear la variable con dicho valor! Por defecto es 0, ante interrupción es 1.
