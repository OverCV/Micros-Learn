param(
    [Parameter(Mandatory=$true)]
    [string]$ProjectName
)

# Crear el directorio del proyecto
New-Item -ItemType Directory -Force -Path $ProjectName

# Crear el archivo .asm
$asmContent = @"
.include "m328Pdef.inc"

.org 0x0000
    rjmp main

main:
    ; Código aquí 🤗

loop:
    rjmp loop
"@

Set-Content -Path "$ProjectName\$ProjectName.asm" -Value $asmContent

# Copiar el Makefile y reemplazar el nombre del proyecto
$makefileContent = Get-Content "Makefile"
$makefileContent = $makefileContent -replace 'PROJECT = main', "PROJECT = $ProjectName"
Set-Content -Path "$ProjectName\Makefile" -Value $makefileContent

Write-Host "Proyecto $ProjectName creado con éxito."