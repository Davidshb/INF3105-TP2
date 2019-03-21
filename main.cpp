#include <iostream>
#include <fstream>
#include "arbreavl.h"

using namespace std;

struct str {
    explicit str();

    ArbreAVL<string> types, foncteurs;
};

str::str() {
    this->types = ArbreAVL<string>();
    this->foncteurs = ArbreAVL<string>();
}

// trim le début des chaînes de caractère
static inline void ltrim(string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void lier(ArbreAVL<string> &arbre, const string &str, const vector<string> &seq) {
    arbre.inserer(str, seq);
}

string getWord(string &cmd) {
    ltrim(cmd);
    string res;

    auto it = cmd.find_first_of(" ={(,)}");
    res = string(cmd, 0, it);
    cmd.erase(0, it);
    return res;
}

void ligne_foncteur(vector<string>& l, string& line,ArbreAVL<string>& types,int taille = 0) {
    string word;
    int taille_base = static_cast<int>(l.size());

    bool virgule = true;

    if(getWord(line) != "(") {
        cerr << "manque ( foncteur" << endl;
        exit(-1);
    }

    while(!(word = getWord(line)).empty()) {
        if(word == ",") {
            if(virgule) {
                cerr << "2 virgules foncteur" << endl;
                exit(-1);
            }
            virgule = true;
            continue;
        }

        if(!virgule) {
            cerr << "0 virgule foncteur" << endl;
            exit(-1);
        }

        if(!types.contient(word)) {
            cerr << "le type n'existe pas" << endl;
            exit(-1);
        }

        l.push_back(word);
        virgule = false;
    }

    if(virgule || (taille && (l.size() - taille_base) != taille - 1 )) {
        cerr << "pas la bonne taille de foncteur" << endl;
        exit(-1);
    }
}

void construire_base_de_donnees(str &arbres, ifstream &file) {
    string line;

    while (getline(file, line)) {
        string word;
        vector<string> l;

        if ((word = getWord(line)) == "type") {
            string identificateur  = getWord(line);
            if (getWord(line) == "=" || getWord(line) == "{") {
                cerr << "manque le =" << endl;
                exit(-1);
            }

            while ((word = getWord(line)) != "}" || !word.empty()) {
                if (word == ",")
                    continue;

                l.push_back(word);
            }

            lier(arbres.types,identificateur,l);
        }else if(word == "foncteur") {
            string identificateur = getWord(line);
            ltrim(line);

            if(line[0] != ':' || line[1] != ':') {
                cerr << "le foncteur manque ::" << endl;
                exit(-1);
            }

            line.erase(0,2);

            ligne_foncteur(l, line, arbres.types);
            int taille = static_cast<int>(l.size());
            l.insert(l.begin(),to_string(taille));


            lier(arbres.foncteurs,identificateur,l);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "manque un argument" << endl;
        exit(-1);
    }

    str arbre;
    ifstream fichier(argv[1], ios::in);

    if (fichier) {
        string line;
        construire_base_de_donnees(arbre,fichier);
        auto it = arbre.types.debut();
        for(;it != arbre.types.fin();it++) {

        }
        fichier.close();
    } else
        cerr << "Erreur d'ouverture de fichier" << endl;

    return 0;
}