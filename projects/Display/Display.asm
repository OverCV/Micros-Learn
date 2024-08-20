    ; Definiciones para el microcontrolador
    .include"m328Pdef.inc"

    .equLCD_RS = 0                                                                                                        ; Pin RS en PORTB
    .equLCD_E = 1                                                                                                         ; Pin E en PORTB
    .equLCD_D4 = 2                                                                                                        ; Pin D4 en PORTB
    .equLCD_D5 = 3                                                                                                        ; Pin D5 en PORTB
    .equLCD_D6 = 4                                                                                                        ; Pin D6 en PORTB
    .equLCD_D7 = 5                                                                                                        ; Pin D7 en PORTB

    .org(0x0000)
    rjmp    init_lcd

    ; Inicialización del LCD
init_lcd:
    ; Configuración de pines como salida
    ldi     r16,            (1<<LCD_RS),    |,  (1<<LCD_E), |, (1<<LCD_D4), |, (1<<LCD_D5), |, (1<<LCD_D6), |, (1<<LCD_D7)
    out     DDRB,           r16

    ; Espera inicial
    ldi     r16,            0x33                                                                                            ; Inicialización del LCD en modo 4 bits
    rcall   lcd_command
    rcall   lcd_command
    ldi     r16,            0x32
    rcall   lcd_command

    ; Configuración del LCD: 4 bits, 2 líneas, 5x8 puntos
    ldi     r16,            0x28
    rcall   lcd_command

    ; Encender la pantalla, cursor apagado
    ldi     r16,            0x0C
    rcall   lcd_command

    ; Incremento automático de dirección, sin desplazamiento
    ldi     r16,            0x06
    rcall   lcd_command

    ; Limpiar la pantalla
    ldi     r16,            0x01
    rcall   lcd_command

    ; Escribir "Over" en la primera línea
    ldi     r16,            0x80                                                                                            ; Dirección de la primera línea
    rcall   lcd_command

    ldi     r16,            'O'
    rcall   lcd_data
    ldi     r16,            'v'
    rcall   lcd_data
    ldi     r16,            'e'
    rcall   lcd_data
    ldi     r16,            'r'
    rcall   lcd_data

    ; Mover el cursor a la segunda línea
    ldi     r16,            0xC0                                                                                            ; Dirección de la segunda línea
    rcall   lcd_command

    ; Escribir "ABC123" en la segunda línea
    ldi     r16,            'A'
    rcall   lcd_data
    ldi     r16,            'B'
    rcall   lcd_data
    ldi     r16,            'C'
    rcall   lcd_data
    ldi     r16,            '1'
    rcall   lcd_data
    ldi     r16,            '2'
    rcall   lcd_data
    ldi     r16,            '3'
    rcall   lcd_data

    rjmp    loop

    ; Bucle principal (nada se hace aquí)
loop:
    rjmp    loop

    ; Subrutina para enviar comandos al LCD
lcd_command:
    cbi     PORTB,          LCD_RS                                                                                          ; RS = 0 para comandos
    rcall   lcd_nibble
    ret

    ; Subrutina para enviar datos al LCD
lcd_data:
    sbi     PORTB,          LCD_RS                                                                                          ; RS = 1 para datos
    rcall   lcd_nibble
    ret

    ; Subrutina para enviar un nibble al LCD
lcd_nibble:
    out     PORTB,          r16                                                                                             ; Enviar nibble
    sbi     PORTB,          LCD_E                                                                                           ; E = 1 (activar enable)
    nop
    cbi     PORTB,          LCD_E                                                                                           ; E = 0 (desactivar enable)
    rcall   lcd_delay                                                                                                       ; Retardo para permitir que el LCD procese el comando
    ret

    ; Subrutina de retardo
lcd_delay:
    ldi     r18,            0xFF
delay_loop:
    dec     r18
    brne    delay_loop
    ret
