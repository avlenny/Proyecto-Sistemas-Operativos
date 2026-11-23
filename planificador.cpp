#include "tipos.h"
#include <deque>
#include <iostream>
#include <algorithm>

void descargarProceso(int id); //Descarga del módulo de memoria.

static std::deque<Bloque> colaRR; //Cola RR (FIFO): cuanto restante de cada proceso. Crea cpias del Bloque.

void rrAgregar(const Bloque& p){ //Proceso entra a cola RR.
  colaRR.push_back(p);
}
const std::deque<Bloque>& rrLista(){ //Devuelve la cola por referencia (útil para la interfaz).
  return colaRR;
}

void rrLimpiar() {
    colaRR.clear();
}
//Ejecuta un ciclo de simulación.
//Devuelve true si el proceso terminó. tiempoAtención contiene los ciclos desde su entrada (estadística de tiempo medio de atención).

bool rrAtender (int quantum, int ciclo, int& tiempoAtencion){ 
  if(colaRR.empty()) return false;
  Bloque p=colaRR.front();//Saca proceso que más tiempo ha esperado. Se sacacde la cola mientras se ejecuta.
  colaRR.pop_front();

  std::cout<<"Atendiendo proceso [" << p.id << "," << p.tam << "," << p.cuanto <<"]\n";
  p.cuanto -=std::min(quantum, p.cuanto); //Se resta el cuanto. Uso min() para no pasar a negativo: si al proceso le queda 1 y el cuanto del sistema es 2, solo consume lo restante.

  if(p.cuanto == 0){//Expira el tiempo, termina y no regresa a la cola
    tiempoAtencion =ciclo - p.cicloEntrada;
    std::cout<<"Descargando proceso ["<<p.id<< "," << p.tam << ",0]\n"; //Se kibera bloque en memoria. El módulo de memoria debe condensar los huecos contiguos aqui.
    descargarProceso (p.id);
    return true;
  }
  colaRR.push_back(p); //Si queda tiempo, vuevlve al final de la cola, así cada proceso recibe su turno "equitativo".
  return false;
}
