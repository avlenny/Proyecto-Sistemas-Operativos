#ifndef TIPOS_H
#define TIPOS_H

//Aquí pongan las funciones que todos vamos a usar, por ejemplo la función de los algoritmos de colocación
const int PRIMER_AJUSTE = 1;
const int PEOR_AJUSTE = 2;

struct Bloque {
    int id;           
    int tam;           
    int cuanto;       
    int cicloEntrada;  
};

struct Config {
    int memoriaKB = 4096;                    
    int quantum = 2;                       
    int maxTam = 500;                     
    int maxCuanto = 10;                      
    int velMs = 1000;   
    int algoritmo = PRIMER_AJUSTE;                 
};

Config pedirConfig();
void mostrarConfig(const Config& cfg);
Bloque genProceso(const Config& cfg, int id, int ciclo);

const int SPLIT_MINIMO_KB = 32;

#endif