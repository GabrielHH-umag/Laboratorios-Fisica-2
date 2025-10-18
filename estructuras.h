#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#define MIN_LINEALIZACION 0
#define MAX_LINEALIZACION 2
#define MIN_ECUACION 0
#define MAX_ECUACION 2

typedef enum
{
    E_Lineal = 0,
    E_Potencial = 1,
    E_Logaritmica = 2,
} Ecuacion;

typedef struct 
{
    Ecuacion tipo_ecuacion;
    double *distancia;
    double *capacitancia;
    double buffer_d;
    double buffer_c;
    double parametro_m;
    double parametro_b;
    double correlacion_lineal;
    double correlacion_modelo;
    size_t size;
} Datos;

#endif 