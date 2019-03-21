#include <utility>

/* UQAM / Département d'informatique
   INF3105 - Structures de données et algorithmes
   Squelette pour classe générique ArbreAVL<T> pour le Lab6 et le TP2.

   AUTEUR(S):
    1) Nom + Code permanent du l'étudiant.e 1
    2) Nom + Code permanent du l'étudiant.e 2
*/

#if !defined(__ARBREAVL_H__)
#define __ARBREAVL_H__

#include <cassert>
#include "pile.h"
#include <vector>

template<class T>
class ArbreAVL {
public:
    ArbreAVL();

    ArbreAVL(const ArbreAVL &);

    ~ArbreAVL();

    // Annonce l'existance d'une classe Iterateur.
    class Iterateur;

    void inserer(const T &, const std::vector<T> &);

    bool contient(const T &) const;

    bool vide() const;

    void vider();

    void enlever(const T &); // non requis pour le TP2.

    int hauteur() const;

    // Fonctions pour obtenir un itérateur (position dans l'arbre)    
    Iterateur debut() const;

    Iterateur fin() const;

    Iterateur rechercher(const T &) const;

    Iterateur rechercherEgalOuSuivant(const T &) const;

    Iterateur rechercherEgalOuPrecedent(const T &) const;

    // Accès aux éléments de l'arbre via un intérateur.
    const T &operator[](const Iterateur &) const;

    T &operator[](const Iterateur &);

    // Copie d'un arbre AVL.
    ArbreAVL &operator=(const ArbreAVL &);

private:
    struct Noeud {
        explicit Noeud(const T &, const std::vector<T> &);

        T contenu;
        std::vector<T> value;
        int equilibre;
        Noeud *gauche, *droite;
    };

    Noeud *racine;

    // Fonctions internes
    bool inserer(Noeud *&, const T &, const std::vector<T> &);

    void rotationGaucheDroite(Noeud *&);

    void rotationDroiteGauche(Noeud *&);

    void vider(Noeud *&);

    void copier(const Noeud *, Noeud *&) const;

    const T &max(Noeud *) const;

    bool enlever(Noeud *&, const T &e);

    int hauteur(Noeud *) const;

public:
    // Sera présenté à la semaine #7
    class Iterateur {
    public:
        Iterateur(const ArbreAVL &a);

        Iterateur(const Iterateur &a);

        Iterateur(const ArbreAVL &a, Noeud *c);

        operator bool() const;

        bool operator!() const;

        bool operator==(const Iterateur &) const;

        bool operator!=(const Iterateur &) const;

        const T &operator*() const;

        Iterateur &operator++();

        Iterateur operator++(int);

        Iterateur &operator=(const Iterateur &);

    private:
        const ArbreAVL &arbre_associe;
        Noeud *courant;
        Pile<Noeud *> chemin;

        friend class ArbreAVL;
    };
};


//-----------------------------------------------------------------------------

template<class T>
ArbreAVL<T>::Noeud::Noeud(const T &c, const std::vector<T> &l)
        : contenu(c), equilibre(0), gauche(NULL), droite(NULL), value(l) {
}

template<class T>
ArbreAVL<T>::ArbreAVL()
        : racine(NULL) {
}

template<class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T> &autre)
        : racine(NULL) {
    copier(autre.racine, racine);
}

template<class T>
ArbreAVL<T>::~ArbreAVL() {
    vider(racine);
}

template<class T>
bool ArbreAVL<T>::contient(const T &element) const {
    Noeud *tmp = racine;

    while (tmp) {
        if (tmp->contenu == element)
            return true;

        if (element < tmp->contenu)
            tmp = tmp->gauche;
        else if (element > tmp->contenu)
            tmp = tmp->droite;
    }

    return false;
}

template<class T>
void ArbreAVL<T>::inserer(const T &element, const std::vector<T> &value) {
    inserer(racine, element, value);
}

template<class T>
bool ArbreAVL<T>::inserer(Noeud *&noeud, const T &element, const std::vector<T> &value) {
    if (noeud == NULL) {
        noeud = new Noeud(element, value);
        return true;
    }
    if (element < noeud->contenu) {
        if (inserer(noeud->gauche, element, value)) {
            noeud->equilibre++;
            if (noeud->equilibre == 0)
                return false;
            if (noeud->equilibre == 1)
                return true;
            assert(noeud->equilibre == 2);
            if (noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
        return false;
    }
    if (noeud->contenu < element) {
        if (inserer(noeud->droite, element, value)) {
            noeud->equilibre--;
            if (noeud->equilibre == 0)
                return false;
            if (noeud->equilibre == -1)
                return true;
            assert(noeud->equilibre == -2);
            if (noeud->droite->equilibre == 1)
                rotationGaucheDroite(noeud->droite);
            rotationDroiteGauche(noeud);
        }
        return false;
    }
    // element == noeud->contenu
    noeud->contenu = element;  // Mise à jour
    return false;
}

template<class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud *&racinesousarbre) {
    Noeud *temp = racinesousarbre->gauche;
    int ea = temp->equilibre;
    int eb = racinesousarbre->equilibre;
    int neb = -(ea > 0 ? ea : 0) - 1 + eb;
    int nea = ea + (neb < 0 ? neb : 0) - 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;
}

template<class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud *&racinesousarbre) {
    // À compléter.
    Noeud *tmp = racinesousarbre->droite;
    int ea = racinesousarbre->equilibre;
    int eb = tmp->equilibre;
    int nea = ea + 1 + (0 < -eb ? -eb : 0);
    int neb = 1 + eb + (0 < nea ? nea : 0);

    tmp->equilibre = neb;
    racinesousarbre->equilibre = nea;
    racinesousarbre->droite = tmp->gauche;
    tmp->gauche = racinesousarbre;
    racinesousarbre = tmp;
}

template<class T>
bool ArbreAVL<T>::vide() const {
    // À compléter.
    return racine == NULL;
}

template<class T>
void ArbreAVL<T>::vider() {
    vider(racine);
}

template<class T>
void ArbreAVL<T>::vider(Noeud *&noeud) {
    if (noeud != NULL) {
        vider(noeud->droite);
        vider(noeud->gauche);
        delete noeud;
    }
    // À compléter.
}

template<class T>
void ArbreAVL<T>::copier(const Noeud *source, Noeud *&noeud) const {
    if (source != NULL) {
        noeud = new Noeud(source->contenu, source->value);
        noeud->equilibre = source->equilibre;
        copier(source->gauche, noeud->gauche);
        copier(source->droite, noeud->droite);
    }
}


template<class T>
int ArbreAVL<T>::hauteur(Noeud *n) const {
    if (n == NULL)
        return 0;

    int a = hauteur(n->gauche), b = hauteur(n->droite);

    return 1 + (a < b ? b : a);
}

template<class T>
int ArbreAVL<T>::hauteur() const {
    return hauteur(racine);
}

template<class T>
const T &ArbreAVL<T>::max(Noeud *n) const {
    if(n == NULL)
        return NULL;
    while(n->droite != NULL)
        n = n->droite;

    return n->contenu;
}

// L'enlèvement est optionnel (non requise pour le TP2)
template<class T>
void ArbreAVL<T>::enlever(const T &element) {
    enlever(racine, element);
}

template<class T>
bool ArbreAVL<T>::enlever(Noeud *&noeud, const T &element) {
    if (element < noeud->contenu) {
        if (enlever(noeud->gauche, element)) {
            noeud->equilibre--;
            if (noeud->equilibre == 0) return false;
            if (noeud->equilibre == -1) return true;
            assert(noeud->equilibre == -2);
            if (noeud->droite->equilibre == 1)
                rotationGaucheDroite(noeud->droite);
            rotationDroiteGauche(noeud);
        }
        return false;
    } else if (element > noeud->contenu) {
        if (enlever(noeud->droite, element)) {
            noeud->equilibre++;
            if (noeud->equilibre == 0) return false;
            if (noeud->equilibre == 1) return true;
            assert(noeud->equilibre == 2);
            if (noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
        return false;
    } else if (element == noeud->contenu) {
        if (noeud->gauche == NULL && noeud->droite == NULL) {
            delete noeud;
            noeud = NULL;
            return true;
        } else {

            return true;
        }
    }
    return false;

}



//----------- L'enlèvement est optionnel (non requise pour le TP2) ----------//

template<class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const {
    Iterateur iter(*this);
    iter.courant = racine;
    if (iter.courant != NULL)
        while (iter.courant->gauche != NULL) {
            iter.chemin.empiler(iter.courant);
            iter.courant = iter.courant->gauche;
        }
    return iter;
}

template<class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const {
    return Iterateur(*this);
}

template<class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T &e) const {
    Iterateur iter(*this);
    Noeud *tmp = racine;
    while (tmp) {
        if (e < tmp->contenu) {
            iter.chemin.empiler(tmp);
            tmp = tmp->gauche;
        } else if (tmp->contenu < e)
            tmp = tmp->droite;
        else {
            iter.courant = tmp;
            return iter;
        }
    }
    iter.chemin.vider();
    return iter;
}

template<class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant(const T &e) const {
    Iterateur iter(*this);
    iter.courant = this->racine;
    Noeud *tmp = nullptr;

    while (iter && iter.courant->contenu != e) {
        if (e > iter.courant->contenu) {
            iter.chemin.empiler(iter.courant);
            iter.courant = iter.courant->gauche;
        }
    }
    // À compléter.
    return iter;
}

template<class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent(const T &e) const {
    // À compléter.
    Iterateur it(*this);
    it.courant = this->racine;

    while (it && it.courant->contenu != e) {
        if (e < it.courant->contenu) {
            it.chemin.empiler(it.courant);
            it.courant = it.courant->droite;
        }
    }
    return it;
}

template<class T>
const T &ArbreAVL<T>::operator[](const Iterateur &iterateur) const {
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template<class T>
T &ArbreAVL<T>::operator[](const Iterateur &iterateur) {
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template<class T>
ArbreAVL<T> &ArbreAVL<T>::operator=(const ArbreAVL &autre) {
    if (this == &autre) return *this;
    vider();
    copier(autre.racine, racine);
    return *this;
}

//-----------------------
template<class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL &a)
        : arbre_associe(a), courant(NULL) {
}

template<class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur &a)
        : arbre_associe(a.arbre_associe) {
    courant = a.courant;
    chemin = a.chemin;
}

// Pré-incrément
template<class T>
typename ArbreAVL<T>::Iterateur &ArbreAVL<T>::Iterateur::operator++() {
    // À compléter.
    if (!*this)
        return *this;

    courant = courant->droite;
    while (*this) {
        chemin.empiler(courant);
        courant = courant->gauche;
    }
    if (!chemin.vide())
        courant = chemin.depiler();
    return *this;
}

// Post-incrément
template<class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int) {
    Iterateur copie(*this);
    operator++();
    return copie;
}

template<class T>
ArbreAVL<T>::Iterateur::operator bool() const {
    return courant != NULL;
}

template<class T>
bool ArbreAVL<T>::Iterateur::operator!() const {
    return courant == NULL;
}

template<class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur &o) const {
    assert(&arbre_associe == &o.arbre_associe);
    return courant == o.courant;
}

template<class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur &o) const {
    assert(&arbre_associe == &o.arbre_associe);
    return courant != o.courant;
}

template<class T>
const T &ArbreAVL<T>::Iterateur::operator*() const {
    assert(courant != NULL);
    return courant->contenu;
}

template<class T>
typename ArbreAVL<T>::Iterateur &ArbreAVL<T>::Iterateur::operator=(const Iterateur &autre) {
    assert(&arbre_associe == &autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

#endif
