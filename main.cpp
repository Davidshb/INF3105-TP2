#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "arbreavl.h"

using namespace std;

// enlève les espaces du début des chaînes de caractère
static void inline ltrim(string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void inline lier(ArbreAVL<string, vector<string>> *arbre, const string &str, const vector<string> &seq) {
    arbre->inserer(str, seq);
}

string getWord(string &cmd) {
    ltrim(cmd);
    string res;

    auto it = cmd.find_first_of(" ={(,)}?", 1);
    res = string(cmd, 0, it);
    cmd.erase(0, it);
    return res;
}

void inline afficher(vector<string> v) {
    for (const string &w : v)
        cout << w << " ";
    cout << endl;
}

void ligne_foncteur(vector<string> &foncteur, string &line, const vector<vector<string>> &types) {
    string word;
    int taille_base = static_cast<int>(foncteur.size());

    bool virgule = true;
    int taille = stoi(foncteur[0]);

    if (line[0] != '(') {
        cerr << "manque ( foncteur" << endl;
        exit(-1);
    }
    line.erase(0, 1);
    int nb = 0;

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

        auto it_fin = types[nb].end();

        if (find(types[nb].begin(), it_fin, word) == it_fin) {
            cerr << "cette valeur " << word << " pour le type " << word << " n'existe pas" << endl;
            exit(-1);
        }

        foncteur.push_back(word);
        virgule = false;
        nb = (nb == taille ? 0 : ++nb);
    }

    if (word != ")") {
        cerr << "ne termine pas par )" << endl;
        exit(-1);
    }

    if (virgule || (foncteur.size() - taille_base) != taille) {
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
ArbreAVL<string, vector<string>> *construire_base_de_donnees(const char *name) {
    ifstream file(name);
    if (!file) {
        cerr << "impossible d'ouvrir le fichier" << endl;
        exit(-1);
    }

    auto *res = new ArbreAVL<string, vector<string>>();
    string line;

    while (getline(file, line)) {
        string word;
        vector<string> l;

        if ((word = getWord(line)) == "type") {
            string identificateur = getWord(line);
            if (res->contient(identificateur)) {
                cerr << "identificateur existe déjà" << endl;
                exit(-1);
            }

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

                if(find(l.begin(),l.end(),word) != l.end()) {
                    cerr << "la valeur " << word << " du type est déjà inclus" << endl;
                    exit(-1);
                }

                virgule = false;

                l.push_back(word);
            }

            lier(res, identificateur, l);
        } else if (word == "foncteur") {
            string identificateur = getWord(line);
            if (res->contient(identificateur)) {
                cerr << "identificatuer existe déja" << endl;
                exit(-1);
            }
            ltrim(line);

            if (line[0] != ':' || line[1] != ':') {
                cerr << "le foncteur manque ::" << endl;
                exit(-1);
            }

            vector<vector<string>> types;
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
                if (!res->contient(word, types)) {
                    cerr << "le type n'existe pas : '" << word << "'" << endl;
                    exit(-1);
                }
                l.push_back(word);
            }
            int taille = static_cast<int>(l.size());
            if (taille != types.size()) {
                cerr << "tailles différentes des types" << endl;
                exit(-1);
            }
            l.insert(l.begin(), to_string(taille));

            streamoff pos = file.tellg();
            while (getline(file, line) && line[0] == '(') {
                pos = file.tellg();
                ligne_foncteur(l, line, types);
            }
            file.seekg(pos);

            lier(res, identificateur, l);
        } else if (!line.empty()) {
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

    //lecture du fchier et creation des structures de données
    ArbreAVL<string, vector<string>> *baseDeDonnees = construire_base_de_donnees(argv[1]);
    string line;
    string word;

    for (auto it = baseDeDonnees->debut(); it != baseDeDonnees->fin(); ++it)
        for (const string &i : (*baseDeDonnees)[it])
            cout << i << endl;

    while (cin && getline(cin, line)) {
        ltrim(line);
        if (line.empty())
            continue;

        word = getWord(line);
        auto it = baseDeDonnees->rechercher(word);
        if (it == baseDeDonnees->fin()) {
            cerr << "identificateur n'est pas reconnu" << endl;
            continue;
        }

        ltrim(line);

        if (line.empty()) {
            cerr << "Il manque la commande" << endl;
            continue;
        }

        const vector<string> &list = (*baseDeDonnees)[it];
        int taille = static_cast<int>(list.size());
        if (line[0] == '?') {
            int taille_argument;

            if ((taille_argument = baseDeDonnees->est_foncteur(it))) {
                for (int i = 1 + taille_argument; i < taille; i++) {
                    if (i % taille_argument == 1)
                        cout << '(';
                    cout << list[i];
                    if (i % taille_argument)
                        cout << ", ";
                    else
                        cout << ')' << endl;
                }
            } else {
                cout << '{';
                for (int i = 0; i < taille; i++) {
                    cout << list[i];
                    if (i + 1 != taille)
                        cout << ", ";
                }
                cout << '}' << endl;
            }
        } else if (line[0] == '(') {
            int taille_argument, nb = -1;

            if (!(taille_argument = baseDeDonnees->est_foncteur(it))) {
                cerr << word << " n'est pas un foncteur" << endl;
                continue;
            }

            line.erase(0, 1);
            bool virgule = true;
            vector<string> tmp;

            while (!line.empty() && line[0] != ')' && !(word = getWord(line)).empty()) {
                if (word == ",") {
                    if (virgule)
                        break;
                    virgule = true;
                    continue;
                }

                if (!virgule) {
                    virgule = true;
                    break;
                }
                virgule = false;
                if (word == "?") {
                    if (nb != -1) {
                        virgule = true;
                        break;
                    }
                    nb = static_cast<int>(tmp.size());
                }
                tmp.push_back(word);
            }

            if (virgule || nb == -1) {
                cerr << "erreur de syntaxe" << endl;
                continue;
            }

            auto vec = list;

            if (tmp.size() != taille_argument) {
                cerr << "mauvaise arité pour le foncteur" << endl;
                continue;
            }

            for (int i = taille_argument + 1, j = 0; i < taille; i++, j = j == taille_argument - 1 ? 0 : j + 1) {
                word = tmp[j];

                if (word == "?")
                    continue;
                else if (vec[i] != word) {
                    vec.erase(vec.begin() + i - j, vec.begin() + i - j + taille_argument);
                    i -= j + 1;
                    j = -1;
                    taille -= taille_argument;
                }
            }

            vec.erase(vec.begin(), vec.begin() + taille_argument + 1);
            taille -= taille_argument + 1;

            vector<string> res;
            for (int i = 0; i < taille; i++)
                if (i % taille_argument == nb)
                    res.push_back(vec[i]);

            taille = static_cast<int>(res.size());

            if(!taille)
                continue;
            cout << '{';
            for(int i = 0; i < taille; i++) {
                cout << res[i];
                if(i != taille - 1)
                    cout << ", ";
            }
            cout << '}' << endl;
        }
    }

    delete baseDeDonnees;

    return 0;
}