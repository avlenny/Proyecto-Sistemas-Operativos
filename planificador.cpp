#include "tipos.h"
#include <deque>
#include <algorithm>

static std::deque<Bloque> colaRR; 

void rrAgregar(const Bloque& p) { 
    colaRR.push_back(p);
}

const std::deque<Bloque>& rrLista() { 
    return colaRR;
}

void rrLimpiar() {
    colaRR.clear();
}

// ejecuta un turno de Round Robin.
bool rrAtender(int quantum, int ciclo, int& tiempoAtencion) {
    if (colaRR.empty()) return false;

    Bloque p = colaRR.front(); 
    colaRR.pop_front();

    mostrarAtencion(p);
    p.cuanto -= std::min(quantum, p.cuanto); 

    if (p.cuanto == 0) {                    
        tiempoAtencion = ciclo - p.cicloEntrada;
        mostrarListaRR();                   
        mostrarDescarga(p);
        descargarProceso(p.id);              
        return true;
    }

    colaRR.push_back(p);                     
    mostrarListaRR();
    return false;
}