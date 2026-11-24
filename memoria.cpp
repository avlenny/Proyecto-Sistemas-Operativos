#include "tipos.h"
#include <vector>
#include <deque>
#include <iostream>

static std::vector<Bloque> memoria; //Memoria ordenada. Hueco=bloque con id 0.

void iniciarMemoria(int memoriaKB) { //Recibe tamaño elegifo por el usuario
    memoria.clear();
    memoria.push_back(Bloque{0, memoriaKB, 0, 0});   //Agrega bloque con id,tam,cuanto,cicloEntrada
}

static int elegirHueco(int tam, int alg) { //Primer o Peor Ajuste
    int elegido = -1; //"Mejor proceso a acomodar"
    for (int i = 0; i < (int)memoria.size(); i++) { //Recorre los bloques de izq a derecha 
        if (memoria[i].id != 0 || memoria[i].tam < tam) continue;  //Bloques que no sirven: ocupado o pequeño

        if (alg == 1) return i;    //Primer Ajuste: El primero en entrar se atiende                  

        if (elegido == -1 || memoria[i].tam > memoria[elegido].tam) //Peor Ajuste: Revisa los bloques y se queda al más grande
            elegido = i;
    }
    return elegido;
}

bool colocarProceso(const Bloque& p, int alg) { //Mete el proceso a la memoria según el algoritmo
    int i = elegirHueco(p.tam, alg); //posición del hueco elegido para el proceso
    if (i == -1) return false;  // No alcanzó el hueco. (El proceso espera)

    int tamHueco = memoria[i].tam; //Guarda tam del hueco antes de modificarlo
    int sobra = tamHueco - p.tam; //Espacio libre después de colocar el proceso

    memoria[i] = p; //El proceso ocupa el inicio del hueco (bloque tam=tam proceso)                                  
    if (sobra >= SPLIT_MINIMO_KB) //Acción sobre lo que sobra
        memoria.insert(memoria.begin() + i + 1, Bloque{0, sobra, 0, 0});  //32+ kb inserta hueco desoues del proceso
    else
        memoria[i].tam = tamHueco;   //32-kb el proceso ocupa todo el hueco                 
    return true; //Proceso colocado
}

//fusiona huecos contiguos para evitar fragmentacion externa
static void condensar() {
    for (int i = 0; i + 1 < (int)memoria.size(); ) { //compara el tamaño de todos los huecos
        if (memoria[i].id == 0 && memoria[i + 1].id == 0) { //verifica que los bloques sean huecos
            memoria[i].tam += memoria[i + 1].tam; //suma tamaño de bloques
            memoria.erase(memoria.begin() + i + 1);   //elimina el segundo bloque absorbido por el primero
        } else {
            i++; //avanza si no hay fusión de bloques
        }
    }
}

void descargarProceso(int id) { //libera bloque de proceso terminado
    for (auto& b : memoria) {
        if (b.id == id) { b.id = 0; b.cuanto = 0; break; } //busca el bloqur con ese id y lo convierte en hueco
    }
    imprimirMemoria();       //memoria con hueco recien creado
    mostrarCondensando();   
    condensar(); //fusiona huecos
}

int memoriaOcupadaKB() { //suma el tamaño de los bloques con id diferente a 0. Ayuda a calcular el % de ocupación de cada ciclo
    int total = 0;
    for (const auto& b : memoria)
        if (b.id != 0) total += b.tam;
    return total;
}

void imprimirMemoria() { //imprime los bloques en el formato [id,tam,cuanto]
    for (const auto& b : memoria) {
        int cuanto = 0;
        if (b.id != 0)
            for (const auto& p : rrLista())
                if (p.id == b.id) { cuanto = p.cuanto; break; }
        std::cout << "[" << b.id << "," << b.tam << "," << cuanto << "]";
    }
    std::cout << "\n";
}
