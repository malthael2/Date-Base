#ifndef TP3_TABLA_H
#define TP3_TABLA_H

#include"Registro.h"

using namespace std;
class Tabla {
public:
    Tabla ();
    Tabla(NombreTabla& NT, vector<NombreCampo>& campos,NombreCampo& clav); // se le pasa por parametro que campo es el campo clave, y los campos que tiene
    ~Tabla();
    vector < NombreCampo > campos();
    Valor campoClave();
    NombreTabla nombre();
    Registro RegistroPorClave (const Valor& v);
    void Insertar( Registro& reg);
    void Borrar(const Valor& v);
    void Nueva(NombreCampo camp);
    vector<pair<NombreCampo, vector<Valor>>> matris ();
    Tabla& operator=(const Tabla& t) ;
    friend class BaseDeDatos;


private:
    NombreTabla nombret;
    NombreCampo clave;
    vector<NombreCampo> camposT;
    string_map<Registro> TabDevalores;
    vector<pair<NombreCampo, vector<Valor>>> matrisReg;
    vector<Registro> registros;
};

#endif