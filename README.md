En linux escriba make all para compilar y ejecutar el codigo.

Para compilar en windows se requiere el uso de MYSYS2
https://www.msys2.org/ para la descarga.
luego de esto ingrese a la aplicacion llamada Mingw64
ejecute los siguientes comandos

    pacman -Syu
    pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-cmake mingw-w64-x86_64-gnuplot git
    git clone --recursive https://github.com/GabrielHH-umag/FisicaLabII-Capacitores-.git
    cd FisicaLabII-Capacitores-
    make build
    make run <Laboratorio que desea ver> (1 o 2)

Para entrar al siguiente grafico presione enter desde la  consola donde se ejecuto
