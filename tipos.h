#ifndef TIPOS_H
#define TIPOS_H

//Aquí pongan las funciones que todos vamos a usar, por ejemplo la función de los algoritmos de colocación

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
};

#endif