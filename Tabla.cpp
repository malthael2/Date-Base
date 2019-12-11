#include"Tabla.h"

Tabla::Tabla(){}

Tabla::Tabla(NombreTabla& NT, vector<NombreCampo>& campos,NombreCampo& clav) : nombret(NT), clave(clav) {
    for(NombreCampo c : campos){
        camposT.push_back(c);
    }
}

Tabla::~Tabla(){}

Valor Tabla::campoClave (){
    return clave;
}

vector<NombreCampo> Tabla:: campos() {
    return camposT;
}

void Tabla :: Insertar( Registro& regis) {
    registros.push_back(regis);
    linear_set<NombreCampo> ncs = regis.campos();
    for( NombreCampo nc : ncs){
        if( nc == this->clave){
            TabDevalores.insert( regis[nc] , regis);
            break;
        }
    }
    if (matrisReg.size() == 0) {
        linear_set<NombreCampo> agregar = regis.campos();
        for (NombreCampo nc : agregar){
            vector<NombreCampo> cord;
            cord.push_back(regis[nc]);
            matrisReg.push_back( make_pair(nc, cord));
        }

    } else {
        for (NombreCampo nc: regis.campos()) {
            for (unsigned int i = 0; i < matrisReg.size(); i++) {
                if (matrisReg[i].first == nc) {
                    matrisReg[i].second.push_back(regis[nc]);
                }
            }
        }
    }
}

Registro Tabla::RegistroPorClave(const Valor& v){
    return TabDevalores[v];
}


NombreTabla Tabla:: nombre(){
    return nombret;
}




void Tabla ::Borrar(const Valor& v){
    TabDevalores.erase(v);
}

Tabla& Tabla::operator=(const Tabla& t) {
    this->registros = t.registros;
    this->TabDevalores = t.TabDevalores;
    this->camposT = t.camposT;
    this->nombret = t.nombret;
    this->matrisReg= t.matrisReg;
    this->clave = t.clave;
    return *this;
}

void Tabla::Nueva (NombreCampo camp){
    camposT.push_back(camp);
}

vector<pair<NombreCampo, vector<Valor>>> Tabla::matris (){
    return matrisReg;
}