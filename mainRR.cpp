#include "tipos.h"
#include <iostream>
#include <conio.h>      
#define NOMINMAX        
#include <windows.h>    //sleep
using namespace std;

static bool esperar(int& velMs) { //espera que el usuario reduzca o aumente la velocidad de simulación, o lo detenga
    int espera = 0;
    while (espera < velMs) {
        while (_kbhit()) {
            int c = _getch();
            if (c == '\r' || c == '\n') return true;
            if (c == '+') velMs = (velMs - 100 < 50) ? 50 : velMs - 100;
            if (c == '-') velMs = (velMs + 100 > 5000) ? 5000 : velMs + 100;
        }
        Sleep(20);
        espera += 20;
    }
    return false;
}

int main() {
    Config cfg = pedirConfig();      //recibe la configuración por parte del usuario//
    int alg = cfg.algoritmo;        

    iniciarMemoria(cfg.memoriaKB); //Inicia la memoria del sistema
    rrLimpiar();

    int ciclo = 0, siguienteId = 1, terminados = 0;
    long long sumaAtencion = 0; //tiempo de atención de procesos terminados
    double sumaOcupacion = 0;        //porcentaje de memoria usada en cada ciclo
    bool detener = false; //paro

    Bloque pendiente = genProceso(cfg, siguienteId++, ciclo);   //genera siguiente proceso

    //Entrada de los procesos a la memoria
    while (!detener) {
        ciclo++;
        if (colocarProceso(pendiente, alg)) {
            pendiente.cicloEntrada = ciclo;
            rrAgregar(pendiente);
            mostrarEntrada(pendiente);
            mostrarMemoria();                              
            pendiente = genProceso(cfg, siguienteId++, ciclo);
        } else {
            mostrarEspera(pendiente); //avisa que el proceso está en espera
        }
        int tiempoAtencion = 0;
        if (rrAtender(cfg.quantum, ciclo, tiempoAtencion)) { //turno RR. Resta min(quantum,cuanto restante).
            //Si sobra tiempo, lo regrega al final de la cola, sino lo descarga de memoria.
            terminados++; //+1 proceso atendido
            sumaAtencion += tiempoAtencion;
        }
        mostrarMemoria();
        sumaOcupacion += memoriaOcupadaKB() * 100.0 / cfg.memoriaKB; //Muestra cómo quedó la memoria y el dato lo suma para las stats de ocupación.

        detener = esperar(cfg.velMs);
    }

    mostrarEstadisticas(ciclo, terminados, sumaAtencion, sumaOcupacion / ciclo); //muestra las stats finales.
    return 0;
}
