#include "tipos.h"
#include <cstdlib> 
#include <ctime>     

static int aleatorio(int minimo, int maximo) {
    static bool norep = false;
    if (!norep) {
        srand((unsigned)time(NULL));
        norep = true;
    }
    return rand() % (maximo - minimo + 1) + minimo;
}

Bloque generarProceso(const Config& cfg, int id, int ciclo) {
    Bloque p;
    p.id = id;
    p.tam = aleatorio(1, cfg.maxTam);       
    p.cuanto = aleatorio(1, cfg.maxCuanto);   
    p.cicloEntrada = ciclo;
    return p;
}
