#include "tipos.h"
#include <deque>
#include <algorithm>

static std::deque<Bloque> colaRR; //cola FIFO procesos ya cargados a memoria

void rrAgregar(const Bloque& p) { //mete un proceso al final de la cola, solo los ya cargados en memoria
    colaRR.push_back(p);
}

const std::deque<Bloque>& rrLista() { //referencia a la cola 
    return colaRR;
}

void rrLimpiar() {
    colaRR.clear();
}

// ejecuta un turno de Round Robin. Base del planificador
bool rrAtender(int quantum, int ciclo, int& tiempoAtencion) {
    if (colaRR.empty()) return false; //si no hay nada en la cola, no hacemos nada

    Bloque p = colaRR.front(); //saca de la cola al proceso que más lleve esperando.
    colaRR.pop_front();

    mostrarAtencion(p);
    p.cuanto -= std::min(quantum, p.cuanto); //resta el tiempo de cpu usado, cuanto nunca queda negativo

    if (p.cuanto == 0) {                    
        tiempoAtencion = ciclo - p.cicloEntrada; //ciclos que pasaron en el sistema
        mostrarListaRR();                   
        mostrarDescarga(p);
        descargarProceso(p.id);  //libera su bloque de memoria, imprime memoria y fusiona huecos            
        return true;
    }

    colaRR.push_back(p);  //de regreso al final de la cola del RR                   
    mostrarListaRR();
    return false;
}
