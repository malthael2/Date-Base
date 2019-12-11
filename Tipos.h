#ifndef __TIPOS_H__
#define __TIPOS_H__

#include <iostream>
#include <string>
#include <vector>
#include "modulos_basicos/linear_set.h"
#include "modulos_basicos/linear_map.h"
#include "modulos_basicos/linear_set_iterators.h"

using namespace std;

typedef string NombreTabla;
typedef string NombreCampo;
typedef string Valor;

#define COMPLETAR(X)    { \
    cerr << "ERROR --- El método " #X " está incompleto." << endl; \
    exit(1); \
}

#endif
