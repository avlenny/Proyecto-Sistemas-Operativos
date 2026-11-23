#ifndef MEMORIA_H
#define MEMORIA_H

#include <vector>
#include "tipos.h"

const int LIBRE = -1;

struct Segmento {
    int id;
    int inicio;
    int tam;
};

void memIniciar(const Config& cfg);
bool memAsignar(const Bloque& p);
void descargarProceso(int id);
const std::vector<Segmento>& memLista();

#endif
