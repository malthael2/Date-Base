#include "BaseDatos.h"

BaseDeDatos::BaseDeDatos() {}

BaseDeDatos::~BaseDeDatos() {}

void BaseDeDatos::agregarTabla(NombreTabla Nt, Tabla& t) {
    BaseDatos.insert(Nt, t);
    tablas.insert(Nt);
}

void BaseDeDatos::EliminarTabla ( NombreTabla Nt){
    BaseDatos.erase(Nt);
    tablas.erase(Nt);
}

void BaseDeDatos:: InsertarRegistro(NombreTabla Nt, Registro& reg){
    BaseDatos[Nt].Insertar(reg);
}

void BaseDeDatos:: EliminarRegistro (NombreTabla Nt, Registro reg) {
    linear_set<NombreCampo> aBorrar = reg.campos();
    for( NombreCampo nc : aBorrar){
        for (unsigned int i = 0; i < BaseDatos[Nt].matrisReg.size(); i++){ // hay que buscar los valores del registro en la matris de valores
            if( nc ==  BaseDatos[Nt].matrisReg[i].first){
                for(unsigned int j = 0; j < BaseDatos[Nt].matrisReg[i].second.size(); j++){ // para cada campo del reg se busca su valor y elimina
                    if ( BaseDatos[Nt].matrisReg[i].second[j] == reg[nc]){
                        BaseDatos[Nt].matrisReg[i].second[j].erase(BaseDatos[Nt].matrisReg[i].second[j].begin() + i);
                        break;
                    }
                }
                break;
            }
        }
    }
    for (unsigned int i = 0; i < BaseDatos[Nt].registros.size(); i++) { // tambien hay que eliminarlo de la lista de registros de la tabla
        if( BaseDatos[Nt].registros[i] == reg) {
            BaseDatos[Nt].registros.erase(BaseDatos[Nt].registros.begin() + i);
            break;
        }
    }
        BaseDatos[Nt].Borrar(reg[BaseDatos[Nt].clave]);
    }

linear_set<Registro>BaseDeDatos::hacerConsulta (const Consulta& q){
    if ( q.tipo_consulta() == FROM){
        return consultaFROM(q.nombre_tabla());

    } else if(q.tipo_consulta() == SELECT){
        if ( q.subconsulta1().tipo_consulta() == SELECT and q.campo1() == BaseDatos[q.subconsulta1().subconsulta1().nombre_tabla()].clave ) { // caso del select con clave de un select sin clave

            return SelectEspecial(q.subconsulta1().subconsulta1(), q.valor());
        }
        return consultaSelect( q.subconsulta1(), q.campo1() , q.valor());
    }
    else if (q.tipo_consulta() == MATCH){

        return consultaMatch(q.subconsulta1(), q.campo1() , q.campo2());
    }
    else if (q.tipo_consulta() == RENAME){

        return consultaRename( q.subconsulta1() , q.campo1() , q.campo2());
    }
    else if (q.tipo_consulta() == UNION){

        return consultaUnion(q.subconsulta1(), q.subconsulta2());
    }
    else if (q.tipo_consulta() == INTER){
        return consultaInter(q.subconsulta1(), q.subconsulta2());
    }
    else if (q.tipo_consulta() == PRODUCT){

        return consultaProduct(q.subconsulta1(), q.subconsulta2());
    }
    else{
        return consultaProj(q.subconsulta1(), q.conj_campos());
    }
}

linear_set<Registro> BaseDeDatos::consultaFROM ( NombreTabla Nt){
    return linear_set<Registro>(BaseDatos[Nt].registros.begin(), BaseDatos[Nt].registros.end());
}




linear_set<Registro> BaseDeDatos::SelectEspecial(const Consulta& q1 , Valor v) {
    linear_set<Registro> res;
    res.fast_insert(BaseDatos[q1.nombre_tabla()].TabDevalores[v]);
    return res;
}

linear_set<Registro> BaseDeDatos::consultaSelect ( const Consulta& q1 , NombreCampo nc, Valor v){
    linear_set<Registro> res;
    if ( q1.tipo_consulta() == FROM){
        if( nc ==  BaseDatos[q1.nombre_tabla()].clave ){ // si el valor es el del campo clave, se lo busca en el trie tabdeValores
            res.fast_insert(BaseDatos[q1.nombre_tabla()].TabDevalores[v]);
        }
        else{
            for( unsigned int i =0; i < BaseDatos[q1.nombre_tabla()].matrisReg.size() ; i++){ // caso contrario se lo busca en la matriz de valores y se agrega el registro que coincide
                if ( BaseDatos[q1.nombre_tabla()].matrisReg[i].first == nc ){
                    for ( unsigned int j =0; j < BaseDatos[q1.nombre_tabla()].matrisReg[i].second.size() ; j++){
                        if(BaseDatos[q1.nombre_tabla()].matrisReg[i].second[j] == v){
                            res.fast_insert(BaseDatos[q1.nombre_tabla()].registros[j]);
                        }
                    }
                }
            }

        }
    }
    else {
        res = hacerConsulta(q1);
        linear_set<Registro> aux;
        for (Registro r : res) {
            if (r[nc] == v) {
                aux.fast_insert(r);
            }
        }
        res = aux;
    }
        return res;
}

Registro UnirRegis (Registro r1, Registro r2){
    linear_set<NombreCampo> aux = r2.campos();
    for(NombreCampo nc : aux){
        if(!r1.definido(r2[nc])){
        r1.Definir(nc, r2[nc]);
    }
    }
    return r1;

}


linear_set<Registro> BaseDeDatos::consultaMatch(const Consulta& q1, NombreCampo campo1, NombreCampo campo2) {
    linear_set<Registro> res;
    if (  q1.tipo_consulta() == PRODUCT and q1.subconsulta1().tipo_consulta() == FROM and q1.subconsulta2().tipo_consulta() == FROM)  {
        if (campo1 == q1.subconsulta1().nombre_tabla() and campo2 == q1.subconsulta2().nombre_tabla()) {// join con claves
            linear_set<Registro> aux1 = hacerConsulta(q1.subconsulta1());
            linear_set<Registro> aux2 = hacerConsulta(q1.subconsulta2());
            if (aux1.size() < aux2.size()) { // para cumplir la complejidad de buscar en el minimo
                for (Registro r : aux1) {
                    Valor v = r[campo1];
                    if (BaseDatos[q1.subconsulta2().nombre_tabla()].TabDevalores.count(v) == 1) { // si el valor esta definido en TabDevalor entonces el campo2 matchea con el campo 2
                        res.fast_insert(UnirRegis(r, BaseDatos[q1.subconsulta2().nombre_tabla()].TabDevalores[v]));
                    }
                }
            } else {
                for (Registro r : aux2) {
                    Valor v = r[campo2];
                    if (BaseDatos[q1.subconsulta1().nombre_tabla()].TabDevalores.count(v) == 1) { // si el valor esta definido en TabDevalor entonces el campo2 matchea con el campo 1
                        res.fast_insert(UnirRegis(r, BaseDatos[q1.subconsulta1().nombre_tabla()].TabDevalores[v]));
                    }
                }
            }
        }
        else {
            linear_set<Registro> aux = hacerConsulta(q1);
            for (Registro r : aux) {
                if (r[campo1] == r[campo2]) {
                    res.fast_insert(r);
                                }
             }
        }
    }
    else {
            linear_set<Registro> aux = hacerConsulta(q1);
            for (Registro r : aux) {
                if (r[campo1] == r[campo2]) {
                    res.fast_insert(r);
                }
            }
        }

    return res;
}

linear_set<Registro> BaseDeDatos::consultaRename(const Consulta& q1, NombreCampo campo1, NombreCampo campo2) {
    linear_set<Registro> res;
    linear_set<Registro> aux = hacerConsulta(q1);
    for( Registro r : aux) {
        if (!r.definido(campo2)) { // si el campo2 no esta definido se procede a agregarlo, si no , no se hace nada
            Valor sig = r[campo1];
            r.borrarCampo(campo1);
            r.Definir(campo2, sig);
            res.fast_insert(r);
        }
    }
    return res;
}

linear_set<Registro> BaseDeDatos::consultaInter(const Consulta& q1, const Consulta& q2) {
    linear_set<Registro> res;
    linear_set<Registro> aux1 = hacerConsulta(q1);
    linear_set<Registro> aux2 = hacerConsulta(q2);

    for (Registro r : aux2) {
            if (aux1.count(r) == 1) {
                res.fast_insert(r);
            }
        }
    return res;
}

linear_set<Registro> BaseDeDatos::consultaUnion(const Consulta& q1, const Consulta& q2) {
    linear_set<Registro> res = hacerConsulta(q1);
    linear_set<Registro> aux = hacerConsulta(q2);
    for(Registro r : aux){
        if ( res.count(r) == 0){
            res.fast_insert(r);
        }
    }
    return res;
}

linear_set<Registro> BaseDeDatos::consultaProduct ( const Consulta& q1 ,const  Consulta& q2){
    linear_set<Registro> res1;
    linear_set<Registro> res = hacerConsulta(q1);
    linear_set<Registro> aux = hacerConsulta(q2);
    for(Registro r : res){
        for(Registro reg: aux) {
            linear_set<NombreCampo> sig = reg.campos();
            for (NombreCampo nc : sig) {
                    r.Definir(nc, reg[nc]);
            }
            res1.fast_insert(r);
        }
    }
    return res1;
}

linear_set<Registro> BaseDeDatos::consultaProj ( const Consulta& q1, set<NombreCampo> campos1){
    linear_set<Registro> res = hacerConsulta(q1);
    for ( Registro r : res){
        linear_set<NombreCampo> sig = r.campos();
        for( NombreCampo nc : sig){
            if ( campos1.count(nc) == 0){
                r.borrarCampo(nc);
            }
        }
    }
    return res;
}


BaseDeDatos& BaseDeDatos::operator=(const BaseDeDatos& bd){
    this->BaseDatos = bd.BaseDatos;
    this->tablas=bd.tablas;
    return *this;
    }
Tabla& BaseDeDatos::operator[] (const NombreTabla t){
    return BaseDatos[t];

}