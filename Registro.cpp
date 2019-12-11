#include "Registro.h"

Registro::Registro() {
}

linear_set <NombreCampo> Registro::campos() const {
    return claves;
}

string& Registro::operator[](const NombreCampo& campo) {
    Valor t;
    if (definido(campo) != 1) {
        Definir(campo, t);
    }

    return reg[campo];
}
void Registro::Definir(NombreCampo nc, Valor& v){
    reg.insert(nc,v);
    claves.insert(nc);
}

bool Registro::definido(NombreCampo nc){
    if (claves.count(nc) != 1) return false;
        else return true;
}

void Registro::borrarCampo(NombreCampo nc){
    claves.erase(nc);
    reg.erase(nc);
}

bool Registro::operator==( const Registro& r) const{
    if ( this->claves == r.claves and this->reg == r.reg ) return true;
    else return false;
}

bool Registro::operator!=( const Registro& r) const {
    return !(*this == r);
}


Registro& Registro::operator=( const Registro& r){
    this->claves = r.claves;
    this->reg = r.reg;
    return *this;
}
