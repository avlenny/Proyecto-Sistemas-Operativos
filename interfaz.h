#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <deque>
#include <string>
#include <vector>
#include "tipos.h"

struct Control {
    int  velocidadMs = 500;
    bool pausado     = false;
    bool parar       = false;
};

void iniciarInterfaz();
void cerrarInterfaz();

void limpiarPantalla();
void mostrarControles(const Control& ctl, int ciclo);

void mostrarMemoria(const std::vector<Bloque>& memoria, int memoriaKB);
void mostrarColaRR(const std::deque<Bloque>& cola, int quantum);

void mostrarEventos(const std::vector<std::string>& eventos);

void esperarTick(Control& ctl);

#endif