#include "tipos.h"
#include <cstdlib> 
#include <ctime>     

static int random(int minimo, int maximo) {
    static bool norep = false;
    if (!norep) {
        srand((unsigned)time(NULL));
        norep = true;
    }
    return rand() % (maximo - minimo + 1) + minimo;
}

Bloque genProceso(const Config& cfg, int id, int ciclo) {
    Bloque p;
    p.id = id;
    p.tam = random(1, cfg.maxTam);       
    p.cuanto = random(1, cfg.maxCuanto);   
    p.cicloEntrada = ciclo;
    return p;
}
