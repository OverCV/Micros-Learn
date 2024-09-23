# Nombre del proyecto
PROJECT = main

# Herramientas AVR y opciones
AVRASM = "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avrassembler\avrasm2.exe"
MCU = atmega2560

# Archivos de salida
TARGET_HEX = $(PROJECT).hex
TARGET_LSS = $(PROJECT).lss
TARGET_OBJ = $(PROJECT).obj

# Archivos de entrada
SRC = $(PROJECT).asm

# Rutas de inclusión
INCLUDE_PATHS = -I"C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.7.374/avrasm/inc" -I"C:/Program Files (x86)/Atmel/Studio/7.0/toolchain/avr8/avrassembler/Include"

# Opciones del ensamblador
ASMFLAGS = -fI -o $(TARGET_HEX) -m $(PROJECT).map -l $(TARGET_LSS) -S $(PROJECT).tmp -W+ie $(INCLUDE_PATHS) -im2560def.inc -d $(TARGET_OBJ)

# Objetivo predeterminado
all: $(TARGET_HEX) clean_temp

# Compilación
$(TARGET_HEX): $(SRC)
	$(AVRASM) $(ASMFLAGS) $(SRC)

# Limpiar archivos temporales
clean_temp:
	del $(TARGET_LSS) $(PROJECT).map $(PROJECT).tmp $(TARGET_OBJ)

# Limpiar todos los archivos generados, incluyendo el fichero .hex
clean:
	del $(TARGET_HEX) $(TARGET_LSS) $(PROJECT).map $(PROJECT).tmp $(TARGET_OBJ)