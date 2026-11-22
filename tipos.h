#ifndef TIPOS_H
#define TIPOS_H

struct Bloque {
    int id;           
    int tam;           
    int quantum;       
    int cicloEntrada;  
};

struct Config {
    int memoriaKB   = 4096;                    
    int quantum     = 2;                       
    int maxTam      = 500;                     
    int maxCuanto   = 10;                      
    int velocidadMs = 1000;                    
};

#endif