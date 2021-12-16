#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "rwstring.hpp"
#include "listasutil.hpp"
#include "listas.hpp"
#include "listasordc.hpp"

using namespace std;

//Declaracion de Constantes a utilizar
const unsigned int CANTIDAD_PLAZOS = 4;
const unsigned int LARGO_NOMBRE = 12;
const unsigned int LARGO_BOLSA = 14;

//Declaracion de estructuras a utilizar
//PUNTO 1
struct Operacion{
    string nombre = "";
    int plazo = 4;
    string bolsa = "";
    float precioUni = 0;
    int cantidad = 0;
};

//PUNTO 2.3
struct Accion{
    string nombre = "";
    int cantidad = 0;
};

//PUNTO 2.2
struct Bolsa{
    string nombre = "";
    double monto = 0;
    Nodo<Accion>* listaAcciones = nullptr;
};

//PUNTO 2.1
struct Plazo{
    string nombre = "";
    int cantidad = 0;
    Nodo<Bolsa>* listaBolsas = nullptr;
};

//PUNTO 1
//Sobrecarga para la impresion de una operacion por consola
ostream& operator << (ostream& os, const Operacion& op){
    os << left << setw(LARGO_NOMBRE + 2) << op.nombre << setw(8) << op.plazo << setw(LARGO_BOLSA + 2) << op.bolsa;
    os << right << setw(10) << setprecision(2) << op.precioUni << setw(11) << op.cantidad;
    return os;
}

//PUNTO 1
//Sobrecarga para la lectura de una operacion del archivo de datos
fstream& operator >> (fstream& fs, Operacion& op){
    op.nombre = readstring(fs, LARGO_NOMBRE);
    fs.read((char*)&op.plazo, sizeof(Operacion::plazo));
    op.bolsa = readstring(fs, LARGO_BOLSA);
    fs.read((char*)&op.precioUni, sizeof(Operacion::precioUni));
    fs.read((char*)&op.cantidad, sizeof(Operacion::cantidad));
    return fs;
}

//PUNTO 2.2
//Sobrecarga para la impresion de una Bolsa por consola
ostream& operator << (ostream& os, const Bolsa& bo){
    os << left << setw(LARGO_BOLSA) << bo.nombre << right << setw(9) << setprecision(2) << bo.monto; 
    return os;
}

//PUNTO 2.3
//Sobrecarga para la impresion de una Accion por consola
ostream& operator << (ostream& os, const Accion& ac){
    os << left << setw(LARGO_NOMBRE + 1) << ac.nombre << right << setw(5) << ac.cantidad; 
    return os;
}

//PUNTO 2.2
//Criterio Ascendente para la comparacion de dos Bolsas
int BolsaAscendente(Bolsa b1, Bolsa b2){
    return b1.nombre.compare(b2.nombre);
}

//PUNTO 2.2 y 2.3
//Sobrecarga de la funcion "mostrar" para adaptarla a la impresion de Bolsas y Acciones
void mostrar (Nodo<Bolsa>* listaBolsas){
    while (listaBolsas != nullptr){
        cout << "BOLSA           MONTO" << endl;
        cout << "========================" << endl;
        cout << listaBolsas->dato << endl;
        cout << "\n" << endl;
        cout << "ACCION    CANTIDAD" << endl;
        cout << "==================" << endl;
        mostrar(listaBolsas->dato.listaAcciones);
        cout << "\n" << endl;
        listaBolsas = listaBolsas->sig;
    }
}

//PUNTO 2.1
//Funcion para la impresion del primer nivel del listado
void mostrarDatos(Plazo lista[]){
    for(int i = 0; i < CANTIDAD_PLAZOS; i++){
        cout << "PLAZO: " << lista[i].nombre << ", CANTIDAD DE OPERACIONES: " << lista[i].cantidad << endl;
        cout << "\n" << endl;
        mostrar(lista[i].listaBolsas);
    }
}

int main(){
    //Declaracion de variables a utilizar
    cout << fixed;
    fstream archivoDatos;
    Operacion op;
    Plazo listaPorPlazo[] = {{"CI", 0}, {"24hs", 0}, {"48hs", 0}, {"72hs", 0}};
    Bolsa bo;
    Accion ac;
    Nodo<Bolsa>* auxBolsa;

    //Control de apertura
    archivoDatos.open("datos.bin", ios::in | ios::binary);
    if(not archivoDatos){
        cout << "Error al abrir el archivo de datos" << endl;
        return EXIT_FAILURE;
    }
    cout << left << setw(LARGO_NOMBRE) << "NOMBRE" << setw(10) << "PLAZO" << setw(LARGO_BOLSA + 5) << "BOLSA" << setw(10) << "PRECIOU" << setw(10) << "CANTIDAD" << endl;
    cout << setfill('=') << setw(59) << "" << setfill(' ') << endl;

    //Lectura de datos
    while(archivoDatos >> op){
        cout << op << endl;
        listaPorPlazo[op.plazo].cantidad++;
        bo.nombre = op.bolsa;
        ac.nombre = op.nombre;
        ac.cantidad = op.cantidad;
        auxBolsa = insertar_unico(bo, listaPorPlazo[op.plazo].listaBolsas, BolsaAscendente);
        auxBolsa->dato.monto += op.cantidad * op.precioUni;
        agregar(auxBolsa->dato.listaAcciones, ac);
    }
    archivoDatos.close();
    cout << setfill('=') << setw(59) << "" << setfill(' ') << endl;
    cout << "\n" << endl;
    cout << "LISTADO" << endl;
    cout << "\n" << endl;

    //Impresion de datos
    mostrarDatos(listaPorPlazo);

    //Libero memoria
    for(int i = 0; i < CANTIDAD_PLAZOS; i++){
        while(listaPorPlazo[i].listaBolsas != nullptr){
            while(listaPorPlazo[i].listaBolsas->dato.listaAcciones != nullptr){
                pop(listaPorPlazo[i].listaBolsas->dato.listaAcciones);
            }
            pop(listaPorPlazo[i].listaBolsas);
        }            
    }
    return EXIT_SUCCESS;
}