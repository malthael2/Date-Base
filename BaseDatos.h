#ifndef TP3_BASEDATOS_H
#define TP3_BASEDATOS_H

#include "Tabla.h"

using namespace std;
class BaseDeDatos {
    public:
    BaseDeDatos();
    ~BaseDeDatos();
    void agregarTabla (NombreTabla Nt, Tabla& t);
    void EliminarTabla ( NombreTabla Nt);
    void InsertarRegistro(NombreTabla Nt, Registro& reg);
    void EliminarRegistro (NombreTabla Nt, Registro reg);
    linear_set<Registro> hacerConsulta(const Consulta& q);
    linear_set<Registro> consultaFROM ( NombreTabla Nt);
    linear_set<Registro> consultaSelect (const  Consulta& q1 , NombreCampo nc, Valor v);
    linear_set<Registro> SelectEspecial (const Consulta& q1 , Valor v);
    linear_set<Registro> consultaMatch( const Consulta& q1, NombreCampo campo1 , NombreCampo campo2);
    linear_set<Registro> consultaRename (const Consulta& q1, NombreCampo campo1 , NombreCampo campo2);
    linear_set<Registro> consultaUnion ( const Consulta& q1 , const Consulta& q2);
    linear_set<Registro> consultaInter ( const Consulta& q1 ,const Consulta& q2);
    linear_set<Registro> consultaProj ( const Consulta& q1, set<NombreCampo> campos1);
    linear_set<Registro> consultaProduct ( const Consulta& q1 , const Consulta& q2);
    BaseDeDatos& operator=(const BaseDeDatos& bd);
    Tabla& operator[] (const NombreTabla t);
    private:
    string_map<Tabla> BaseDatos;
    set<NombreTabla> tablas;
};

#endif
