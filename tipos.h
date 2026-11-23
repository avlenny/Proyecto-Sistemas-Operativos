#ifndef TIPOS_H
#define TIPOS_H

#include <deque>

// Aqui van las funciones que todos usamos.
const int PRIMER_AJUSTE = 1;
const int PEOR_AJUSTE = 2;

struct Bloque {
    int id;
    int tam;
    int cuanto;
    int cicloEntrada;
};

struct Config {
    int memoriaKB = 4096;
    int quantum = 2;
    int maxTam = 500;
    int maxCuanto = 10;
    int velMs = 5000;
    int algoritmo = PRIMER_AJUSTE;
};

const int SPLIT_MINIMO_KB = 32;

// ---- config.cpp / generadorPro.cpp ----
Config pedirConfig();
void mostrarConfig(const Config& cfg);
Bloque genProceso(const Config& cfg, int id, int ciclo);

// ---- planificador.cpp (Round Robin) ----
void rrAgregar(const Bloque& p);
const std::deque<Bloque>& rrLista();
void rrLimpiar();
bool rrAtender(int quantum, int ciclo, int& tiempoAtencion);

// ---- memoria.cpp (Primer y Peor ajuste) ----
void iniciarMemoria(int memoriaKB);
bool colocarProceso(const Bloque& p, int alg);   // false si no cabe
void descargarProceso(int id);                   // libera y condensa huecos
void imprimirMemoria();                          // imprime [id,tam,cuanto][...]
int  memoriaOcupadaKB();                         // KB ocupados por procesos

// ---- interfaz.cpp (TODA la salida a pantalla) ----
void mostrarCiclo(int ciclo);
void mostrarEntrada(const Bloque& p);
void mostrarEspera(const Bloque& p);
void mostrarAtencion(const Bloque& p);
void mostrarDescarga(const Bloque& p);
void mostrarCondensando();
void mostrarListaRR();
void mostrarMemoria();
void mostrarEstadisticas(int ciclos, int terminados, long long sumaAtencion,
                         double ocupacionPromedio);

#endif