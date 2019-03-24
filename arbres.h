#ifndef TP2_ARBRES_H
#define TP2_ARBRES_H

#include <string>
#include <vector>

using namespace std;

struct arbres {
    explicit arbres();

    ArbreAVL<string, vector<string>> types, foncteurs;
};

arbres::arbres() {
    this->types = ArbreAVL<string, vector<string>>();
    this->foncteurs = ArbreAVL<string, vector<string>>();
}

#endif //TP2_ARBRES_H
