#include <utility>

/* UQAM / Département d'informatique
   INF3105 - Structures de données et algorithmes
   Squelette pour classe générique ArbreAVL<K> pour le Lab6 et le TP2.

   AUTEUR(S):
    1) Nom + Code permanent du l'étudiant.e 1
    2) Nom + Code permanent du l'étudiant.e 2
*/

#if !defined(__ARBREAVL_H__)
#define __ARBREAVL_H__

#include <cassert>
#include "pile.h"

inline bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

template<class K, class V>
class ArbreAVL {
public:
    ArbreAVL();

    ArbreAVL(const ArbreAVL &);

    ~ArbreAVL();

    // Annonce l'existance d'une classe Iterateur.
    class Iterateur;

    void inserer(const K &, const V &);

    bool contient(const K &, std::vector<V> &) const;

    bool contient(const K &) const;

    void vider();

    const int est_foncteur(const Iterateur &) const;

    // Fonctions pour obtenir un itérateur (position dans l'arbre)    
    Iterateur debut() const;

    Iterateur fin() const;

    Iterateur rechercher(const K &) const;

    // Accès aux éléments de l'arbre via un intérateur.
    const V &operator[](const Iterateur &) const;

    V &operator[](const Iterateur &);

    const V &operator[](const K &) const;

    V &operator[](const K &);

    // Copie d'un arbre AVL.
    ArbreAVL &operator=(const ArbreAVL &);

private:
    struct Noeud {
        explicit Noeud(const K &, const V & = nullptr);

        K cle;
        V valeur;
        Noeud *gauche, *droite;
        int equilibre;
    };

    Noeud *racine;

    // Fonctions internes
    bool inserer(Noeud *&, const K &, const V &);

    void rotationGaucheDroite(Noeud *&);

    void rotationDroiteGauche(Noeud *&);

    void vider(Noeud *&);

    void copier(const Noeud *, Noeud *&) const;

public:
    // Sera présenté à la semaine #7
    class Iterateur {
    public:
        explicit Iterateur(const ArbreAVL &a);

        Iterateur(const Iterateur &a);

        explicit operator bool() const;

        bool operator!() const;

        bool operator==(const Iterateur &) const;

        bool operator!=(const Iterateur &) const;

        Iterateur &operator++();

        const Iterateur operator++(int);

        Iterateur &operator=(const Iterateur &);

    private:
        const ArbreAVL &arbre_associe;
        Noeud *courant;
        Pile<Noeud *> chemin;

        friend class ArbreAVL;
    };
};


//-----------------------------------------------------------------------------

template<class K, class V>
ArbreAVL<K, V>::Noeud::Noeud(const K &c, const V &l)
        : cle(c), valeur(l), gauche(NULL), droite(NULL), equilibre(0) {
}

template<class K, class V>
ArbreAVL<K, V>::ArbreAVL()
        : racine(NULL) {
}

template<class K, class V>
ArbreAVL<K, V>::ArbreAVL(const ArbreAVL<K, V> &autre)
        : racine(NULL) {
    copier(autre.racine, racine);
}

template<class K, class V>
ArbreAVL<K, V>::~ArbreAVL() {
    vider(racine);
}

template<class K, class V>
bool ArbreAVL<K, V>::contient(const K &cle, std::vector<V> &t) const {
    Noeud *tmp = racine;

    while (tmp) {
        if (tmp->cle == cle && !is_number(tmp->valeur[0])) {
            t.push_back(tmp->valeur);
            return true;
        }

        if (cle < tmp->cle)
            tmp = tmp->gauche;
        else if (cle > tmp->cle)
            tmp = tmp->droite;
    }

    return false;
}

template<class K, class V>
void ArbreAVL<K, V>::inserer(const K &cle, const V &valeur) {
    inserer(racine, cle, valeur);
}

template<class K, class V>
bool ArbreAVL<K, V>::inserer(Noeud *&noeud, const K &cle, const V &valeur) {
    if (noeud == NULL) {
        noeud = new Noeud(cle, valeur);
        return true;
    }
    if (cle < noeud->cle) {
        if (inserer(noeud->gauche, cle, valeur)) {
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
    if (noeud->cle < cle) {
        if (inserer(noeud->droite, cle, valeur)) {
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
    // cle == noeud->cle
    //noeud->cle = cle;  // Mise à jour
    return false;
}

template<class K, class V>
void ArbreAVL<K, V>::rotationGaucheDroite(Noeud *&racinesousarbre) {
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

template<class K, class V>
void ArbreAVL<K, V>::rotationDroiteGauche(Noeud *&racinesousarbre) {
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

template<class K, class V>
void ArbreAVL<K, V>::vider() {
    vider(racine);
}

template<class K, class V>
void ArbreAVL<K, V>::vider(Noeud *&noeud) {
    if (noeud != NULL) {
        vider(noeud->droite);
        vider(noeud->gauche);
        delete noeud;
    }
    // À compléter.
}

template<class K, class V>
void ArbreAVL<K, V>::copier(const Noeud *source, Noeud *&noeud) const {
    if (source != NULL) {
        noeud = new Noeud(source->cle, source->valeur);
        noeud->equilibre = source->equilibre;
        copier(source->gauche, noeud->gauche);
        copier(source->droite, noeud->droite);
    }
}

template<class K, class V>
typename ArbreAVL<K, V>::Iterateur ArbreAVL<K, V>::debut() const {
    Iterateur iter(*this);
    iter.courant = racine;
    if (iter.courant != NULL)
        while (iter.courant->gauche != NULL) {
            iter.chemin.empiler(iter.courant);
            iter.courant = iter.courant->gauche;
        }

    return iter;
}

template<class K, class V>
typename ArbreAVL<K, V>::Iterateur ArbreAVL<K, V>::fin() const {
    return Iterateur(*this);
}

template<class K, class V>
typename ArbreAVL<K, V>::Iterateur ArbreAVL<K, V>::rechercher(const K &e) const {
    Iterateur iter(*this);
    Noeud *tmp = racine;
    while (tmp) {
        if (e < tmp->cle) {
            iter.chemin.empiler(tmp);
            tmp = tmp->gauche;
        } else if (tmp->cle < e)
            tmp = tmp->droite;
        else {
            iter.courant = tmp;
            return iter;
        }
    }
    iter.chemin.vider();
    return iter;
}

template<class K, class V>
const V &ArbreAVL<K, V>::operator[](const Iterateur &iterateur) const {
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->valeur;
}

template<class K, class V>
V &ArbreAVL<K, V>::operator[](const Iterateur &iterateur) {
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->valeur;
}

template<class K, class V>
ArbreAVL<K, V> &ArbreAVL<K, V>::operator=(const ArbreAVL &autre) {
    if (this == &autre) return *this;
    vider();
    copier(autre.racine, racine);
    return *this;
}

template<class K, class V>
const V &ArbreAVL<K, V>::operator[](const K &c) const {
    typename ArbreAVL<K, V>::Iterateur iter = rechercher(c);
    return this[iter];
}

template<class K, class V>
V &ArbreAVL<K, V>::operator[](const K &c) {
    typename ArbreAVL<K, V>::Iterateur iter = rechercher(c);
    if (!iter) {
        inserer(c);
        iter = rechercher(c);
    }
    return this[iter];
}

template<class K, class V>
bool ArbreAVL<K, V>::contient(const K & cle) const {
    Noeud *tmp = racine;

    while (tmp) {
        if (tmp->cle == cle) {
            return true;
        }

        if (cle < tmp->cle)
            tmp = tmp->gauche;
        else if (cle > tmp->cle)
            tmp = tmp->droite;
    }

    return false;
}

template<class K, class V>
const int ArbreAVL<K, V>::est_foncteur(const ArbreAVL::Iterateur &iter) const {
    int res = 0;
    assert(&iter.arbre_associe == this);
    assert(iter.courant);
    if(is_number(iter.courant->valeur[0]))
        res = std::stoi(iter.courant->valeur[0]);
    return res;
}

//-----------------------
template<class K, class V>
ArbreAVL<K, V>::Iterateur::Iterateur(const ArbreAVL &a)
        : arbre_associe(a), courant(NULL) {
}

template<class K, class V>
ArbreAVL<K, V>::Iterateur::Iterateur(const ArbreAVL<K, V>::Iterateur &a)
        : arbre_associe(a.arbre_associe) {
    courant = a.courant;
    chemin = a.chemin;
}

// Pré-incrément
template<class K, class V>
typename ArbreAVL<K, V>::Iterateur &ArbreAVL<K, V>::Iterateur::operator++() {
    // À compléter.
    assert(courant);

    Noeud *suivant = courant->droite;
    while (suivant) {
        chemin.empiler(suivant);
        suivant = suivant->gauche;
    }

    courant = chemin.vide() ? NULL : chemin.depiler();
    return *this;
}

// Post-incrément
template<class K, class V>
const typename ArbreAVL<K, V>::Iterateur ArbreAVL<K, V>::Iterateur::operator++(int) {
    Iterateur copie(*this);
    operator++();
    return copie;
}

template<class K, class V>
ArbreAVL<K, V>::Iterateur::operator bool() const {
    return courant != NULL;
}

template<class K, class V>
bool ArbreAVL<K, V>::Iterateur::operator!() const {
    return courant == NULL;
}

template<class K, class V>
bool ArbreAVL<K, V>::Iterateur::operator==(const Iterateur &o) const {
    assert(&arbre_associe == &o.arbre_associe);
    return courant == o.courant;
}

template<class K, class V>
bool ArbreAVL<K, V>::Iterateur::operator!=(const Iterateur &o) const {
    assert(&arbre_associe == &o.arbre_associe);
    return courant != o.courant;
}

template<class K, class V>
typename ArbreAVL<K, V>::Iterateur &ArbreAVL<K, V>::Iterateur::operator=(const Iterateur &autre) {
    assert(&arbre_associe == &autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

#endif
