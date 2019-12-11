#ifndef __REGISTRO_H__
#define __REGISTRO_H__

#include "string_map.h"
#include "Tipos.h"
#include "Consulta.h"

using namespace std;

class Registro {
public:
    Registro();
    linear_set<NombreCampo> campos() const;
    string& operator[](const NombreCampo& campo);
    void Definir(NombreCampo nc, Valor& v);
    bool definido(NombreCampo nc);
    void borrarCampo(NombreCampo nc);
    Registro& operator=(const Registro& r);
    bool operator==( const Registro& r) const;
    bool operator!=( const Registro& r) const;

    //Registro(const Registro& otro);

private:
    string_map<Valor> reg;
    linear_set<NombreCampo> claves;
};

#endif
