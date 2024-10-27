param(
    [Parameter(Mandatory=$true)]
    [string]$ProjectName
)

# Crear el directorio del proyecto
New-Item -ItemType Directory -Force -Path $ProjectName

# Crear el archivo .asm
$asmContent = @"
    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

main:
    ; Código aquí 🤗

reset:
    ; Inicializar

loop:
    RJMP    loop

.org(0x0100)
table:
    ; Tabla de datos
    ; .dw

"@

Set-Content -Path "$ProjectName\$ProjectName.asm" -Value $asmContent

# Copiar el Makefile y reemplazar el nombre del proyecto
$makefileContent = Get-Content "Makefile"
$makefileContent = $makefileContent -replace 'PROJECT = main', "PROJECT = $ProjectName"
Set-Content -Path "$ProjectName\Makefile" -Value $makefileContent

Write-Host "Proyecto $ProjectName creado con éxito."