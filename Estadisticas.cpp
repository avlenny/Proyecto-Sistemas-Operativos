#include "estadisticas.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

using namespace std;

void registrarCreado(Estadisticas& e, const Bloque& p, int quantum) {
    if (quantum < 1) quantum = 1;
    int turnos = (p.cuanto + quantum - 1) / quantum;
    e.creados++;
    e.info[p.id] = make_pair(p.cicloEntrada, turnos);
}

void registrarSinEspacio(Estadisticas& e) { e.sinEspacio++; }

void registrarTerminado(Estadisticas& e, int id, int cicloFin) {
    auto it = e.info.find(id);
    if (it == e.info.end()) return;
    int llegada = it->second.first;
    int turnos  = it->second.second;
    int retorno = cicloFin - llegada + 1;
    int espera  = retorno - turnos;
    e.terminados++;
    e.sumaRetorno += retorno;
    e.sumaEspera  += max(0, espera);
}

void registrarTick(Estadisticas& e, const vector<Bloque>& memoria,
                   int memoriaKB) {
    if (memoriaKB <= 0) return;

    int usada = 0, libre = 0, mayorHueco = 0, huecos = 0;

    for (const Bloque& b : memoria) {
        if (b.id == 0) {
            libre += b.tam;
            mayorHueco = max(mayorHueco, b.tam);
            huecos++;
        } else {
            usada += b.tam;
        }
    }

    double frag = 0;
    if (libre > 0) frag = 100.0 * (1.0 - (double)mayorHueco / libre);

    e.ticks++;
    e.sumaUso  += 100.0 * usada / memoriaKB;
    e.sumaFrag += frag;
    e.huecosMax = max(e.huecosMax, huecos);
}

static double promedio(double suma, long cuantos) {
    return cuantos > 0 ? suma / cuantos : 0.0;
}

void mostrarEstadisticas(const Estadisticas& e) {
    cout << fixed << setprecision(1);
    cout << "\n---------- ESTADISTICAS FINALES ----------\n";
    cout << "Ciclos simulados ........... " << e.ticks << "\n";
    cout << "Procesos creados ........... " << e.creados << "\n";
    cout << "Procesos terminados ........ " << e.terminados << "\n";
    cout << "Procesos sin terminar ...... " << e.creados - e.terminados << "\n";
    cout << "Ciclos sin espacio ......... " << e.sinEspacio << "\n";
    // Los promedios son: (suma de todos) / (cuantos fueron)
    cout << "Retorno promedio ........... "
         << promedio((double)e.sumaRetorno, e.terminados) << " ciclos\n";
    cout << "Espera promedio ............ "
         << promedio((double)e.sumaEspera, e.terminados) << " ciclos\n";
    cout << "Uso de memoria promedio .... "
         << promedio(e.sumaUso, e.ticks) << " %\n";
    cout << "Fragmentacion promedio ..... "
         << promedio(e.sumaFrag, e.ticks) << " %\n";
    cout << "Maximo de huecos a la vez .. " << e.huecosMax << "\n";
    cout << "----------------------------------------------\n";
}
