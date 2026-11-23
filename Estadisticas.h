#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include <map>
#include <utility>
#include <vector>
#include "tipos.h"

struct Estadisticas {
    int  creados     = 0;
    int  terminados  = 0;
    int  sinEspacio  = 0;
    long ticks       = 0;
    long sumaEspera  = 0;
    long sumaRetorno = 0;
    double sumaUso   = 0;
    double sumaFrag  = 0;
    int  huecosMax   = 0;
    std::map<int, std::pair<int, int>> info;
};

void registrarCreado(Estadisticas& e, const Bloque& p, int quantum);
void registrarSinEspacio(Estadisticas& e);
void registrarTerminado(Estadisticas& e, int id, int cicloFin);
void registrarTick(Estadisticas& e, const std::vector<Bloque>& memoria, int memoriaKB);

void mostrarEstadisticas(const Estadisticas& e);

#endif
