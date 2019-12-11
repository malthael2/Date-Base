#include "string_map.h"

template<typename T>
string_map<T>::string_map(): raiz(NULL), _size(0) {}

template<typename T>
string_map<T>::string_map(const string_map<T> &aCopiar)
        : string_map() { *this = aCopiar; } // Provisto por la catedra: utiliza el operador asignacion para realizar la copia.

template<typename T>
string_map<T> &string_map<T>::operator=(const string_map<T> &d) {
    if (raiz != NULL) {
        eliminar(raiz);
    }
    if (d.raiz != NULL) {
        raiz = copiaNodo(d.raiz);
    }
    this->_size = d.size();
    this->llaves = d.llaves;
    return *this;
}

template<typename T>
typename string_map<T>::Nodo *string_map<T>::copiaNodo(typename string_map<T>::Nodo *n) {
    Nodo *nuevo = new Nodo();
    for (unsigned int i = 0; i < n->siguientes.size(); ++i) {
        if (n->siguientes[i] != NULL) {
            nuevo->siguientes[i] = copiaNodo(n->siguientes[i]);
        }
    }

    if (n->definicion != NULL) {
        T *nuevaDef = new T();
        *nuevaDef = *n->definicion;
        nuevo->definicion = nuevaDef;
    }
    return nuevo;
}

template<typename T>
string_map<T>::~string_map() {
    if (raiz != NULL) {
        eliminar(raiz);
    }
}

template<typename T>
void string_map<T>::eliminar(string_map<T>::Nodo *&n) {
    for (unsigned int i = 0; i < n->siguientes.size(); ++i) {
        if (n->siguientes[i] != NULL) {
            eliminar(n->siguientes[i]);
        }
    }

    if (n->definicion != NULL) {
        delete (n->definicion);
    }
    delete (n);
}

template<typename T>
T &string_map<T>::operator[](const string &clave) {
    T t;
    if (this->count(clave) != 1) {
        this->insert(clave, t);
    }
    return this->at(clave);
}

template<typename T>
int string_map<T>::count(const string &clave) const {
    int contador = 0;
    Nodo *actual = raiz;
    if (actual != NULL) {
        for (unsigned int i = 0; i < clave.size(); ++i) {
            if (actual->siguientes[int(clave[i])] == NULL) {
                return contador;
            }
            actual = actual->siguientes[int(clave[i])];
        }
        if (actual->definicion != NULL) {
            contador = 1;
        }
    }
    return contador;
}

template<typename T>
const T &string_map<T>::at(const string &clave) const {
    Nodo *actual = raiz;
    for (unsigned int i = 0; i < clave.size(); ++i) {
        actual = actual->siguientes[int(clave[i])];
    }
    return *actual->definicion;
}

template<typename T>
T &string_map<T>::at(const string &clave) {
    Nodo *actual = raiz;
    for (unsigned int i = 0; i < clave.size(); ++i) {
        actual = actual->siguientes[int(clave[i])];
    }
    return *actual->definicion;
}

template<typename T>
int string_map<T>::cantDescendientes(string_map<T>::Nodo *&n) {
    int cantHijos = 0;
    for (unsigned int i = 0; i < n->siguientes.size(); ++i) {
        if (n->siguientes[i] != NULL) {
            cantHijos++;
        }
    }
    return cantHijos;
}

template<typename T>
void string_map<T>::erase(const string &clave) {
    llaves.erase(clave);
    Nodo *actNodo = raiz;
    Nodo *ultNodo = raiz;
    int aPartir = 0;
    for (unsigned i = 0; i < clave.size(); ++i) {
        if (cantDescendientes(actNodo) > 1 || actNodo->definicion != NULL) {
            ultNodo = actNodo;
            aPartir = i;
        }
        actNodo = actNodo->siguientes[int(clave[i])];
    }

    delete actNodo->definicion;
    actNodo->definicion = NULL;

    if (cantDescendientes(actNodo) == 0) {
        Nodo *borrar = ultNodo->siguientes[int(clave[aPartir])];
        ultNodo->siguientes[int(clave[aPartir])] = NULL;
        ultNodo = borrar;

        for (unsigned int i = aPartir + 1; i < clave.size(); ++i) {
            borrar = ultNodo->siguientes[int(clave[i])];
            delete (ultNodo);
            ultNodo = borrar;
        }
        delete (ultNodo);
    }
}

template<typename T>
int string_map<T>::size() const {
    return _size;
}

template<typename T>
bool string_map<T>::empty() const {
    bool diccionarioVacio = false;
    if (raiz == NULL) {
        diccionarioVacio = true;
    }
    return diccionarioVacio;
}

template<typename T>
void string_map<T>::insert(string clav, T sig) {
    llaves.insert(clav);
    if (raiz == NULL) {
        Nodo *nuevo = new Nodo();
        raiz = nuevo;
    }
    Nodo *actual = raiz;
    for (unsigned int i = 0; i < clav.size(); ++i) {
        if (actual->siguientes[int(clav[i])] != NULL) {
            actual = actual->siguientes[int(clav[i])];
        } else {
            Nodo *nuevo = new Nodo();
            actual->siguientes[int(clav[i])] = nuevo;
            actual = nuevo;
        }
    }

    if (actual->definicion == NULL) {
        T *def = new T(sig);
        actual->definicion = def;
        _size++;
    } else {
        *actual->definicion = sig;
    }
}

template<typename T>
bool string_map<T>::operator==(const string_map<T> &d) const {

    if (this->llaves == d.llaves) {
        for (string s : llaves) {
            if (d.count(s) != 1) return false;
            else if (this->at(s) != d.at(s)) return false;
        }
        return true;

    } else return false;

};
