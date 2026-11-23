#include "tipos.h"
#include <iostream>
using namespace std;

// Imprime un proceso con el formato del PDF: [id,tam,cuanto]
static void imprimirProceso(const Bloque& p) {
    cout << "[" << p.id << "," << p.tam << "," << p.cuanto << "]";
}

void mostrarCiclo(int ciclo) {
    cout << "\n===== CICLO " << ciclo << " =====\n";
}

void mostrarEntrada(const Bloque& p) {
    cout << "Entra Proceso ";
    imprimirProceso(p);
    cout << "\n";
}

void mostrarEspera(const Bloque& p) {
    cout << "Proceso ";
    imprimirProceso(p);
    cout << " espera: no cabe\n";
}

void mostrarAtencion(const Bloque& p) {
    cout << "Atendiendo proceso ";
    imprimirProceso(p);
    cout << "\n";
}

void mostrarDescarga(const Bloque& p) {
    cout << "Descargando proceso [" << p.id << "," << p.tam << ",0]\n";
}

void mostrarCondensando() {
    cout << "Condensando huecos\n";
}

void mostrarListaRR() {
    cout << "Lista RR";
    for (const auto& p : rrLista()) {
        cout << " ";
        imprimirProceso(p);
    }
    cout << "\n";
}

void mostrarMemoria() {
    imprimirMemoria();   // la memoria es privada de memoria.cpp
}

/*void mostrarVelocidad(int velMs) {
    cout << "(velocidad: " << velMs << " ms por ciclo)\n";
}*/

void mostrarEstadisticas(int ciclos, int terminados, long long sumaAtencion,
                         double ocupacionPromedio) {
    cout << "\n--- Estadisticas ---\n"
         << "Ciclos ejecutados: " << ciclos << "\n"
         << "Procesos atendidos: " << terminados << "\n";
    if (terminados > 0)
        cout << "Tiempo medio de atencion: "
             << (double)sumaAtencion / terminados << " ciclos\n";
    cout << "Ocupacion promedio de memoria: " << ocupacionPromedio << " %\n";
}