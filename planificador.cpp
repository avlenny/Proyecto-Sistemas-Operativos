#include "tipos.h"
#include <deque>
#include <algorithm>

static std::deque<Bloque> colaRR; // Cola RR (FIFO): cuanto restante de cada proceso.

void rrAgregar(const Bloque& p) { // Proceso entra a cola RR.
    colaRR.push_back(p);
}

const std::deque<Bloque>& rrLista() { // Devuelve la cola por referencia.
    return colaRR;
}

void rrLimpiar() {
    colaRR.clear();
}

// Ejecuta un turno de Round Robin.
// Devuelve true si el proceso termino. tiempoAtencion = ciclos desde su entrada.
bool rrAtender(int quantum, int ciclo, int& tiempoAtencion) {
    if (colaRR.empty()) return false;

    Bloque p = colaRR.front(); // El que mas tiempo lleva esperando.
    colaRR.pop_front();

    mostrarAtencion(p);
    p.cuanto -= std::min(quantum, p.cuanto); // min() para no pasar a negativo.

    if (p.cuanto == 0) {                     // Termino: no regresa a la cola.
        tiempoAtencion = ciclo - p.cicloEntrada;
        mostrarListaRR();                    // la lista ya sin el proceso (como en el PDF)
        mostrarDescarga(p);
        descargarProceso(p.id);              // libera el bloque y condensa huecos
        return true;
    }

    colaRR.push_back(p);                     // Le queda tiempo: al final de la cola.
    mostrarListaRR();
    return false;
}