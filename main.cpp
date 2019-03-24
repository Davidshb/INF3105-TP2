#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "arbreavl.h"
#include "arbres.h"

using namespace std;

// enlève les espaces du début des chaînes de caractère
static inline void ltrim(string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void lier(ArbreAVL<string, vector<string>> &arbre, const string &str, const vector<string> &seq) {
    arbre.inserer(str, seq);
}

string getWord(string &cmd) {
    ltrim(cmd);
    string res;

    auto it = cmd.find_first_of(" ={(,)}", 1);
    res = string(cmd, 0, it);
    cmd.erase(0, it);
    return res;
}

void afficher(vector<string> v) {
    for(const string &w : v)
        cout << w << " - ";
    cout << endl;
}

void ligne_foncteur(vector<string> &foncteur, string &line, ArbreAVL<string, vector<string>> types) { //TODO : passer la liste des vecteurs de types au lieu de foncteur'arbre type
    // void ligne_foncteur(vector<string> &foncteur, string &line, vector<string> *types) {
    string word;
    int taille_base = static_cast<int>(foncteur.size());

    bool virgule = true;
    int taille = stoi(foncteur[0]);

    if (line[0] != '(') {
        cerr << "manque ( foncteur" << endl;
        exit(-1);
    }
    line.erase(0, 1);
    int nb = 1;

    while (!(word = getWord(line)).empty() & word != ")") {
        if (word == ",") {
            if (virgule) {
                cerr << "2 virgules foncteur" << endl;
                exit(-1);
            }
            virgule = true;
            continue;
        }

        if (!virgule) {
            cerr << "0 virgule foncteur : " << word << endl;
            exit(-1);
        }

        if (!types.contient(foncteur[nb],word)) {
            cerr << "cette valeur " << word << " pour le type " << foncteur[nb] << " n'existe pas" << endl;
            exit(-1);
        }

        foncteur.push_back(word);
        virgule = false;
        nb = (nb == taille ? 1 : ++nb);
    }

    if(word != ")") {
        cerr << "ne termine pas par )" << endl;
        exit(-1);
    }

    if (virgule || (taille && (foncteur.size() - taille_base) != taille)) {
        cerr << "pas la bonne taille de foncteur " << foncteur.size() << " " << taille << endl;
        exit(-1);
    }
}

bool mot_correct(const string &s) {
    bool res = true;
    if (s == "foncteur" || s == "type")
        res = false;

    for (char it : s)
        if (!isalpha(it))
            res = false;

    return res;
}

/*
 * Cette méthode lit le fichier et construit les arbres de types et de foncteurs
 */
arbres *construire_base_de_donnees(char *name) {
    ifstream file(name);
    if (!file) {
        cerr << "impossible d'ouvrir le fichier" << endl;
        exit(-1);
    }

    auto *res = new arbres();
    string line;

    while (getline(file, line)) {
        string word;
        vector<string> l;

        if ((word = getWord(line)) == "type") {
            string identificateur = getWord(line);

            bool virgule = true;
            if (getWord(line) != "=" || line[1] != '{') {
                cerr << "manque le =" << endl;
                exit(-1);
            }

            line.erase(0, 2);

            while ((word = getWord(line)) != "}" && !word.empty()) {
                if (word == ",") {
                    if (virgule) {
                        cerr << ", de trop" << endl;
                        exit(-1);
                    }
                    virgule = true;
                    continue;
                }

                if (!virgule) {
                    cerr << "manque , " << endl;
                    exit(-1);
                }

                if (!mot_correct(word)) {
                    cerr << "mot incorrect" << endl;
                    exit(-1);
                }

                virgule = false;

                l.push_back(word);
            }

            lier(res->types, identificateur, l);
        } else if (word == "foncteur") {
            string identificateur = getWord(line);
            ltrim(line);

            if (line[0] != ':' || line[1] != ':') {
                cerr << "le foncteur manque ::" << endl;
                exit(-1);
            }

            line.erase(0, 3);
            bool virgule = true;
            while (!line.empty() && line[1] != '(' && !(word = getWord(line)).empty()) {
                if (word == ",") {
                    if (virgule) {
                        cerr << "2 virgules foncteur def" << endl;
                        exit(-1);
                    }
                    virgule = true;
                    continue;
                }

                if (!virgule) {
                    cerr << "pas 2 virgules foncteur def " << word << endl;
                    exit(-1);
                }
                virgule = false;
                if (!res->types.contient(word)) {
                    cerr << "le type n'existe pas : '" << word << "'" << endl;
                    exit(-1);
                }
                l.push_back(word);
            }
            int taille = static_cast<int>(l.size());
            l.insert(l.begin(), to_string(taille));

            streamoff pos = file.tellg();
            while (getline(file, line) && line[0] == '(') {
                pos = file.tellg();
                ligne_foncteur(l, line, res->types);
            }
            file.seekg(pos);

            lier(res->foncteurs, identificateur, l);
        }else {
            cerr << "Jcompren pas qoé" << endl;
            exit(-1);
        }
    }
    file.close();
    return res;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Utilisation: " << argv[0] << " <file_name>" << endl;
        exit(-1);
    }

    arbres *arbres = construire_base_de_donnees(argv[1]);

    for (auto it = arbres->foncteurs.debut(); it != arbres->foncteurs.fin(); ++it)
        for (const string &i : arbres->foncteurs[it])
            cout << i << endl;

    for (auto it = arbres->types.debut(); it != arbres->types.fin(); ++it) {
        for (const string &i : arbres->types[it])
            cout << i;
    }


    delete arbres;

    return 0;
}