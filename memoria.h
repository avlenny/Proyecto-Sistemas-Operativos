#ifndef MEMORIA_H
#define MEMORIA_H

#include <vector>
#include "tipos.h"     

enum class Estrategia { PRIMER_AJUSTE, PEOR_AJUSTE };

class Memoria {
public:
    Memoria(int total, Estrategia e);
    bool asignar(int pid, int tamano);   
    bool liberar(int pid);              
    void condensar();
    const std::vector<Bloque>& getBloques() const { return bloques; }  

private:
    int total;
    Estrategia estrategia;
    std::vector<Bloque> bloques;        
    int buscarHueco(int tamano) const;
};

#endif
