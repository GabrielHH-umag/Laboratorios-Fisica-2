#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "estructuras.h"
#include <math.h>
#include <matplot/matplot.h>
#include <iostream>
#include <string>
#include <vector>
int initialize_data(Datos *data) 
{
    data->distancia = NULL;
    data->capacitancia = NULL;
    data->buffer_d = 0.0;
    data->buffer_c = 0.0;
    data->size = 0;
    data->parametro_m = 0.0;
    data->parametro_b = 0.0;
    data->correlacion_lineal = 0.0;
    data->correlacion_modelo = 0.0;
    data->tipo_ecuacion = E_Lineal;
    return 0;
}
int free_data(Datos *data) 
{
    free(data->distancia);
    free(data->capacitancia);
    data->distancia = NULL;
    data->capacitancia = NULL;
    data->size = 0;
    data->parametro_m = 0.0;
    data->parametro_b = 0.0;
    data->correlacion_lineal = 0.0;
    data->correlacion_modelo = 0.0;
    data->tipo_ecuacion = E_Lineal;
    return 0;
}
int open_file(FILE **file, const char *filename) 
{
    *file = fopen(filename, "r");
    if (*file == NULL) 
    {
        perror("Error al abrir el archivo");
        return 1;
    }
    return 0;
}
int asign_data(FILE **file, Datos *data) 
{
    if (fscanf(*file, "Tipo de ecuacion: %d", (int *)&data->tipo_ecuacion) != 1 || data->tipo_ecuacion < MIN_ECUACION || data->tipo_ecuacion > MAX_ECUACION)
    {
        fprintf(stderr, "Error leyendo el tipo de ecuacion\n");
        free_data(data);
        fclose(*file);
        return 1;
    }
    while(fscanf(*file, "%lf\t%lf", &data->buffer_d, &data->buffer_c) == 2)
    {
        data->size++;
        data->distancia = (double *) realloc(data->distancia, data->size * sizeof(double));
        data->capacitancia = (double *) realloc(data->capacitancia, data-> size * sizeof(double));
        if (data->distancia == NULL || data->capacitancia == NULL)
        {
            perror("Error reallocating memory");
            fclose(*file);
            free_data(data);
            return 1;
        }
        data->distancia[data->size - 1] = data->buffer_d;
        data->capacitancia[data->size - 1] = data->buffer_c;
    }
    fclose(*file);
    return 0;
}
int crear_parametros_de_correlacion(Datos *data)
{
    if (!data || data->size < 2) 
    {
        fprintf(stderr, "Error: datos insuficientes para correlación\n");
        return 1;
    }

    size_t n = data->size;
    std::vector<double> x(data->distancia, data->distancia + n);
    std::vector<double> y(data->capacitancia, data->capacitancia + n);

    // Transformar según tipo de ecuación
    std::vector<double> X(n), Y(n);
    switch (data->tipo_ecuacion)
    {
        case E_Lineal:
        {
            X = x;
            Y = y;
            break;
        }
        case E_Potencial:
        {
            for (size_t i = 0; i < n; ++i) 
            {
                if (x[i] <= 0 || y[i] <= 0) 
                {
                    fprintf(stderr, "Error: valores no positivos en ajuste potencial\n");
                    return 1;
                }
                X[i] = log(x[i]);
                Y[i] = log(y[i]);
            }
            break;
        }
        case E_Logaritmica:
        {
            for (size_t i = 0; i < n; ++i) 
            {
                if (x[i] <= 0) {
                    fprintf(stderr, "Error: valores no positivos en ajuste logarítmico\n");
                    return 1;
                }
                X[i] = log(x[i]);
                Y[i] = y[i];
            }
            break;
        }
        default:
        {
            fprintf(stderr, "Error: tipo de ecuación desconocido\n");
            return 1;
        }
    }
    // Calcular regresión lineal sobre X y Y
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    for (size_t i = 0; i < n; i++) 
    {
        sum_x += X[i];
        sum_y += Y[i];
        sum_xy += X[i] * Y[i];
        sum_x2 += X[i] * X[i];
    }

    double denom = n * sum_x2 - sum_x * sum_x;
    if (denom == 0) 
    {
        fprintf(stderr, "Error: denominador nulo en regresión\n");
        return 1;
    }

    double m = (n * sum_xy - sum_x * sum_y) / denom;
    double b = (sum_y - m * sum_x) / n;

    // Calcular R² del modelo
    double mean_y = sum_y / n;
    double ss_tot = 0, ss_res = 0;
    for (size_t i = 0; i < n; i++) 
    {
        double y_pred = m * X[i] + b;
        ss_tot += pow(Y[i] - mean_y, 2);
        ss_res += pow(Y[i] - y_pred, 2);
    }
    double r2 = 1 - ss_res / ss_tot;

    // Guardar parámetros en escala original
    switch (data->tipo_ecuacion)
    {
        case E_Lineal:
            data->parametro_m = m;
            data->parametro_b = b;
            break;
        case E_Potencial:
            data->parametro_m = exp(b); 
            data->parametro_b = m;      
            break;
        case E_Logaritmica:
            data->parametro_m = m;  
            data->parametro_b = b;
            break;
    }

    data->correlacion_modelo = r2;
    // Calcular R² lineal sobre los datos originales
    sum_x = 0; sum_y = 0; sum_xy = 0; sum_x2 = 0;
    for (size_t i = 0; i < n; i++) 
    {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
    }
    double m_lin = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    double b_lin = (sum_y - m_lin * sum_x) / n;
    mean_y = sum_y / n;
    ss_tot = 0; ss_res = 0;
    for (size_t i = 0; i < n; i++) 
    {
        double y_pred = m_lin * x[i] + b_lin;
        ss_tot += pow(y[i] - mean_y, 2);
        ss_res += pow(y[i] - y_pred, 2);
    }
    data->correlacion_lineal = 1 - ss_res / ss_tot;
    return 0;
}
int grafica(Datos *data, const char *t, const char *independent, const char *dependent)
{
    using namespace matplot;
    std::vector<double> x(data->distancia, data->distancia + data->size);
    std::vector<double> y(data->capacitancia, data->capacitancia + data->size);

    figure();

    if(crear_parametros_de_correlacion(data) != 0)
    {
        fprintf(stderr, "Error creando parámetros de correlación\n");
        return 1;
    }

    // Generar 50 puntos para la línea de tendencia para que se vea fluida
    std::vector<double> x_line;
    std::vector<double> y_line;
    double xmin = *std::min_element(x.begin(), x.end());
    double xmax = *std::max_element(x.begin(), x.end());
    int N = 50;
    double dx = (xmax - xmin) / (N-1);
    for(int i = 0; i < N; ++i) 
    {
        double xi = xmin + i*dx;
        double yi;
        switch(data->tipo_ecuacion) 
        {
            case E_Lineal:    yi = data->parametro_m * xi + data->parametro_b; break;
            case E_Potencial: yi = data->parametro_m * pow(xi, data->parametro_b); break;
            case E_Logaritmica: yi = data->parametro_m * log(xi) + data->parametro_b; break;
            default: yi = 0; break;
        }
        x_line.push_back(xi);
        y_line.push_back(yi);
    }
    // Graficar puntos y línea de tendencia, se usa hold para mantener ambos en la misma figura
    hold(on);
    plot(x_line, y_line, "--")->line_width(2).color({1,0,0}); // línea de tendencia
    scatter(x, y, 6)->marker_face_color({0,0,1}); // puntos
    hold(off);
    title(std::string(t));
    xlabel(std::string(independent));
    ylabel(std::string(dependent));
    grid(true);
    //Para mostrar la ecuacion adecuada y R2
    std::string eq_text= "y =";
    switch(data->tipo_ecuacion) {
        case E_Lineal:
            eq_text += std::to_string(data->parametro_m) + "x + " + std::to_string(data->parametro_b);
            break;
        case E_Potencial:
            eq_text += std::to_string(data->parametro_m) + "x Elevado a (" + std::to_string(data->parametro_b) + ")";
            break;
        case E_Logaritmica:
            eq_text += std::to_string(data->parametro_m) + "ln(x) + " + std::to_string(data->parametro_b);
            break;
    }
    // Posición del texto en la gráfica
    double y_max = *std::max_element(y.begin(), y.end());
    double y_min = *std::min_element(y.begin(), y.end());
    double x_pos = xmin + 0.05*(xmax-xmin);
    double y_pos = y_max - 0.05*(y_max - y_min);
    double x_pos_r2m = xmin + 0.05*(xmax - xmin);
    double y_pos_r2m = y_max - 0.15*(y_max - y_min);
    double x_pos_r2l = xmin + 0.05*(xmax - xmin);
    double y_pos_r2l = y_max - 0.25*(y_max - y_min);
    text(x_pos, y_pos, eq_text);
    text(x_pos_r2l, y_pos_r2l, "R² lineal = " + std::to_string(data->correlacion_lineal));
    text(x_pos_r2m, y_pos_r2m, "R² del modelo = " + std::to_string(data->correlacion_modelo));

    show();
    //no se uso Faradio debido a que matplot no soporta datos tan pequeños
    return 0;
}
