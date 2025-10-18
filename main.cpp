#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"
#include "funciones.h"

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    Datos data;
    if(argc <2||argc>2)
    {
        printf("Error al asignar el laboratorio. asegurese se usar make <laboratorio que quiere>\n");
        return 1;
    }
    initialize_data(&data);
    if(strcmp(argv[1],"1")==0)
    {
        //lab 1
         open_file(&file, "data_sin_linealizar_1.txt");
        asign_data(&file, &data);
        grafica(&data, "Capacitancia en función de la Distancia", "Distancia (m)", "Capacitancia (pF)");
        free_data(&data);
        open_file(&file, "data_linealizada_1.txt");
        asign_data(&file, &data);
        grafica(&data, "Datos linealizados con d ⁻¹ de Capacitancia en función de la Distancia", "Distancia⁻¹ (m⁻¹)", "Capacitancia (pF)");
    }
    if(strcmp(argv[1],"2")==0)
    {
        //lab 2
        open_file(&file, "data_sin_ampolleta_2.txt");
        asign_data(&file, &data);
        grafica(&data, "Voltaje en función de la Corriente", "Corriente(a)", "Voltaje (V)");
        free_data(&data);
        open_file(&file, "data_con_ampolleta_sin_lin2.txt");
        asign_data(&file, &data);
        grafica(&data, "Voltaje en función de la Corriente (Con ampolleta)", "Corriente(A)", "Voltaje (V)");
        free_data(&data);
        open_file(&file, "data_con_ampolleta_lin2.txt");
        asign_data(&file, &data);
        grafica(&data, "Voltaje linealizado en función de la Corriente (Con ampolleta)", "Corriente⁻¹(A⁻¹)", "Voltaje (V)");
    }

   
    free_data(&data);
    return 0;
}