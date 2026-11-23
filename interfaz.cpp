#include "interfaz.h"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#ifdef _WIN32
    #define NOMINMAX
    #include <conio.h>
    #include <windows.h>
#else
    #include <sys/select.h>
    #include <termios.h>
    #include <unistd.h>
#endif

using namespace std;

const string ROJO     = "\033[31m";
const string VERDE    = "\033[32m";
const string AMARILLO = "\033[33m";
const string CIAN     = "\033[36m";
const string NEGRITA  = "\033[1m";
const string NORMAL   = "\033[0m";

const int VEL_MIN  = 50;
const int VEL_MAX  = 5000;
const int VEL_PASO = 100;
const int MAX_COLA = 8;

static char letra(int id) {
    return 'A' + ((id + 25) % 26);
}

#ifdef _WIN32

static bool hayTecla()  { return _kbhit() != 0; }
static char leerTecla() { return (char)_getch(); }

static void iniciarTeclado() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    if (GetConsoleMode(h, &modo)) {
        SetConsoleMode(h, modo | 0x0004);
    }
}
static void cerrarTeclado() {}

#else

static termios configOriginal;
static bool    configGuardada = false;

static bool hayTecla() {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    timeval espera = {0, 0};
    return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &espera) > 0;
}
static char leerTecla() {
    char c = 0;
    if (read(STDIN_FILENO, &c, 1) != 1) c = 0;
    return c;
}
static void iniciarTeclado() {
    if (tcgetattr(STDIN_FILENO, &configOriginal) == 0) {
        configGuardada = true;
        termios nueva = configOriginal;
        nueva.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &nueva);
    }
}
static void cerrarTeclado() {
    if (configGuardada) tcsetattr(STDIN_FILENO, TCSANOW, &configOriginal);
}
#endif

void iniciarInterfaz() { iniciarTeclado(); }
void cerrarInterfaz()  { cerrarTeclado();  }

void limpiarPantalla() {
    cout << "\033[H\033[J" << flush;
}

void mostrarControles(const Control& ctl, int ciclo) {
    cout << NEGRITA << "--- SIMULADOR ROUND ROBIN ---" << NORMAL
         << "   Ciclo: " << ciclo
         << "   Velocidad: " << ctl.velocidadMs << " ms"
         << "   Estado: " << (ctl.pausado ? "PAUSADO" : "CORRIENDO") << "\n";
    cout << "Teclas:  [+] mas rapido   [-] mas lento   "
         << "[P] pausa   [Q] terminar\n\n";
}

void mostrarMemoria(const vector<Bloque>& memoria, int memoriaKB) {
    if (memoriaKB <= 0) return;

    const int ANCHO = 60;
    int usada = 0, libre = 0, huecos = 0;

    cout << NEGRITA << "MEMORIA (" << memoriaKB << " KB)" << NORMAL << "\n";

    cout << "[";
    for (const Bloque& b : memoria) {
        int n = b.tam * ANCHO / memoriaKB;
        if (n < 1) n = 1;
        if (b.id == 0) {
            cout << VERDE << string(n, '.') << NORMAL;
        } else {
            cout << ROJO << string(n, letra(b.id)) << NORMAL;
        }
    }
    cout << "]\n\n";

    cout << "  " << left << setw(8) << "Inicio" << setw(8) << "Tamano"
         << "Contenido\n";
    int inicio = 0;
    for (const Bloque& b : memoria) {
        cout << "  " << left << setw(8) << inicio << setw(8) << b.tam;
        if (b.id == 0) {
            cout << VERDE << "hueco libre" << NORMAL << "\n";
            libre += b.tam;
            huecos++;
        } else {
            cout << ROJO << "P" << b.id << " (" << letra(b.id) << ")"
                 << NORMAL << "\n";
            usada += b.tam;
        }
        inicio += b.tam;
    }

    cout << "\n  Usada: " << usada << " KB (" << usada * 100 / memoriaKB
         << "%)   Libre: " << libre << " KB   Huecos: " << huecos << "\n\n";
}

void mostrarColaRR(const deque<Bloque>& cola, int quantum) {
    cout << NEGRITA << "COLA ROUND ROBIN" << NORMAL
         << " (quantum = " << quantum << ")\n  ";

    if (cola.empty()) {
        cout << "(vacia)\n\n";
        return;
    }

    size_t mostrar = min(cola.size(), (size_t)MAX_COLA);
    for (size_t i = 0; i < mostrar; i++) {
        const Bloque& p = cola[i];
        string color = (i == 0) ? AMARILLO : CIAN;
        cout << color << "[P" << p.id << " (" << letra(p.id)
             << ") resta " << p.cuanto << "]" << NORMAL;
        if (i + 1 < mostrar) cout << " -> ";
    }
    if (cola.size() > mostrar)
        cout << " ... (+" << cola.size() - mostrar << " mas)";
    cout << "\n  ^ siguiente en CPU   (en cola: " << cola.size() << ")\n\n";
}

void mostrarEventos(const vector<string>& eventos) {
    cout << NEGRITA << "EVENTOS DE ESTE CICLO" << NORMAL << "\n";
    if (eventos.empty()) cout << "  (nada)\n";
    for (const string& e : eventos) cout << "  - " << e << "\n";
    cout << "\n";
}

static void procesarTecla(char c, Control& ctl) {
    switch (c) {
        case '+': case '=':
            ctl.velocidadMs = max(VEL_MIN, ctl.velocidadMs - VEL_PASO);
            break;
        case '-': case '_':
            ctl.velocidadMs = min(VEL_MAX, ctl.velocidadMs + VEL_PASO);
            break;
        case 'p': case 'P':
            ctl.pausado = !ctl.pausado;
            break;
        case 'q': case 'Q':
            ctl.parar = true;
            break;
        default:
            break;
    }
}

void esperarTick(Control& ctl) {
    cout << flush;

    int esperado = 0;
    while (esperado < ctl.velocidadMs && !ctl.pausado && !ctl.parar) {
        this_thread::sleep_for(chrono::milliseconds(20));
        esperado += 20;
        if (hayTecla()) procesarTecla(leerTecla(), ctl);
    }

    if (ctl.pausado && !ctl.parar) {
        cout << AMARILLO << "  >>> PAUSADO: presiona [P] para continuar "
             << "o [Q] para terminar" << NORMAL << "\n" << flush;
    }
    while (ctl.pausado && !ctl.parar) {
        this_thread::sleep_for(chrono::milliseconds(50));
        if (hayTecla()) procesarTecla(leerTecla(), ctl);
    }
}