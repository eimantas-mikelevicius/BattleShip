#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Pozicija {
    int eilute;
    int stulpelis;
};

class Objektas {
protected:
    string pavadinimas;

public:
    Objektas(const string& pav) : pavadinimas(pav) {}

    const string& gautiPavadinima() const {
        return pavadinimas;
    }

    virtual void atspausdinti() const {
        cout << "Objektas: " << pavadinimas << endl;
    }

    virtual ~Objektas() {}
};

class Laivas : public Objektas {
private:
    int dydis;
    int pataikymai;
    vector<Pozicija> pozicijos;

public:
    Laivas(const string& pav, int dyd)
        : Objektas(pav), dydis(dyd), pataikymai(0) {
    }

    void pridetiPozicija(int eilute, int stulpelis) {
        Pozicija p;
        p.eilute = eilute;
        p.stulpelis = stulpelis;
        pozicijos.push_back(p);
    }

    bool uzimaLangeli(int eilute, int stulpelis) const {
        for (int i = 0; i < (int)pozicijos.size(); i++) {
            if (pozicijos[i].eilute == eilute && pozicijos[i].stulpelis == stulpelis)
                return true;
        }
        return false;
    }

    bool pataikyti(int eilute, int stulpelis) {
        if (uzimaLangeli(eilute, stulpelis)) {
            pataikymai++;
            return true;
        }
        return false;
    }

    bool yraNumustas() const {
        return pataikymai >= dydis;
    }

    int gautiDydi() const {
        return dydis;
    }

    void atspausdinti() const override {
        cout << "Laivas: " << pavadinimas << " (dydis " << dydis << ")" << endl;
    }
};

class Lenta {
public:
    static const int DYDIS = 10;

private:
    vector<vector<char>> tinklelis;
    vector<Laivas> laivai;

public:
    Lenta() : tinklelis(DYDIS, vector<char>(DYDIS, '.')) {}

    bool galiIsdestyti(int eilute, int stulpelis, int dydis, bool horizontalus) const {
        for (int i = 0; i < dydis; i++) {
            int e = horizontalus ? eilute : eilute + i;
            int s = horizontalus ? stulpelis + i : stulpelis;

            if (e < 0 || e >= DYDIS || s < 0 || s >= DYDIS)
                return false;

            for (int de = -1; de <= 1; de++) {
                for (int ds = -1; ds <= 1; ds++) {
                    int ke = e + de;
                    int ks = s + ds;
                    if (ke >= 0 && ke < DYDIS && ks >= 0 && ks < DYDIS) {
                        if (tinklelis[ke][ks] != '.')
                            return false;
                    }
                }
            }
        }
        return true;
    }

    void isdestytiLaiva(Laivas& laivas, int eilute, int stulpelis, bool horizontalus) {
        for (int i = 0; i < laivas.gautiDydi(); i++) {
            int e = eilute;
            int s = stulpelis;
            if (horizontalus)
                s = stulpelis + i;
            else
                e = eilute + i;

            tinklelis[e][s] = 'L';
            laivas.pridetiPozicija(e, s);
        }
        laivai.push_back(laivas);
    }

    void isdestytiAtsitkitinai(vector<pair<string, int>> laivuSarasas) {
        for (int i = 0; i < (int)laivuSarasas.size(); i++) {
            Laivas laivas(laivuSarasas[i].first, laivuSarasas[i].second);
            int eilute, stulpelis;
            bool horizontalus;
            do {
                eilute = rand() % DYDIS;
                stulpelis = rand() % DYDIS;
                horizontalus = rand() % 2;
            } while (!galiIsdestyti(eilute, stulpelis, laivas.gautiDydi(), horizontalus));
            isdestytiLaiva(laivas, eilute, stulpelis, horizontalus);
        }
    }

    string gautiSuvima(int eilute, int stulpelis) {
        if (tinklelis[eilute][stulpelis] == 'X' || tinklelis[eilute][stulpelis] == 'O')
            return "jau";

        for (int i = 0; i < (int)laivai.size(); i++) {
            if (laivai[i].pataikyti(eilute, stulpelis)) {
                tinklelis[eilute][stulpelis] = 'X';
                if (laivai[i].yraNumustas())
                    return "nuskandintas " + laivai[i].gautiPavadinima();
                return "pataikyta";
            }
        }
        tinklelis[eilute][stulpelis] = 'O';
        return "praleista";
    }

    bool visiNumusti() const {
        for (int i = 0; i < (int)laivai.size(); i++) {
            if (!laivai[i].yraNumustas())
                return false;
        }
        return true;
    }

    void atspausdinti(bool sleptiFlaivus = false) const {
        cout << "  ";
        for (int s = 0; s < DYDIS; s++)
            cout << s << " ";
        cout << endl;

        for (int e = 0; e < DYDIS; e++) {
            cout << e << " ";
            for (int s = 0; s < DYDIS; s++) {
                char simbolis = tinklelis[e][s];
                if (sleptiFlaivus && simbolis == 'L')
                    simbolis = '.';
                cout << simbolis << " ";
            }
            cout << endl;
        }
    }
};

class Zaidejas : public Objektas {
protected:
    Lenta lenta;

public:
    Zaidejas(const string& vardas) : Objektas(vardas) {}

    Lenta& gautiLenta() { return lenta; }
    const Lenta& gautiLenta() const { return lenta; }

    virtual Pozicija gautiSuvima() const = 0;

    void atspausdinti() const override {
        cout << "Žaid?jas: " << pavadinimas << endl;
    }
};

class Zmogus : public Zaidejas {
public:
    Zmogus(const string& vardas) : Zaidejas(vardas) {}

    void isdestytiRankiniu(vector<pair<string, int>> laivuSarasas) {
        for (int i = 0; i < (int)laivuSarasas.size(); i++) {
            Laivas laivas(laivuSarasas[i].first, laivuSarasas[i].second);
            lenta.atspausdinti();
            cout << endl << "Isdestyk " << laivuSarasas[i].first
                << " (dydis " << laivuSarasas[i].second << ")" << endl;

            int eilute, stulpelis;
            char kryptis;
            bool horizontalus;

            while (true) {
                cout << "Iveskite eilute (0-9): ";   cin >> eilute;
                cout << "Iveskite stulpeli (0-9): "; cin >> stulpelis;
                cout << "Horizontalus? (h/v): ";      cin >> kryptis;
                horizontalus = (kryptis == 'h' || kryptis == 'H');

                if (lenta.galiIsdestyti(eilute, stulpelis, laivas.gautiDydi(), horizontalus)) {
                    lenta.isdestytiLaiva(laivas, eilute, stulpelis, horizontalus);
                    break;
                }
                cout << "Negalima isdestyti bandykite dar karta." << endl;
            }
        }
    }

    Pozicija gautiSuvima() const override {
        Pozicija p;
        cout << pavadinimas << ", iveskite suvimo eilute (0-9): ";   cin >> p.eilute;
        cout << pavadinimas << ", iveskite suvimo stulpeli (0-9): "; cin >> p.stulpelis;
        return p;
    }
};

class Kompiuteris : public Zaidejas {
public:
    Kompiuteris() : Zaidejas("Kompiuteris") {}

    Pozicija gautiSuvima() const override {
        Pozicija p;
        p.eilute = rand() % Lenta::DYDIS;
        p.stulpelis = rand() % Lenta::DYDIS;
        return p;
    }
};

class Zaidimas {
private:
    Zmogus zmogus;
    Kompiuteris kompiuteris;

    vector<pair<string, int>> laivuSarasas = {
        {"Keturvieti",  4},
        {"Trivieti",    3},
        {"Trivieti",    3},
        {"Dvivieti",    2},
        {"Dvivieti",    2},
        {"Dvivieti",    2},
        {"Vienvieti",   1},
        {"Vienvieti",   1},
        {"Vienvieti",   1},
        {"Vienvieti",   1}
    };

public:
    Zaidimas() : zmogus("Žaid?jas") {}

    void paruosti() {
        cout << "=== JURU MUSIS ===" << endl << endl;
        cout << "--- Isdestykit savo laivus ---" << endl;
        zmogus.isdestytiRankiniu(laivuSarasas);
        kompiuteris.gautiLenta().isdestytiAtsitkitinai(laivuSarasas);
        cout << endl << "Kompiuteris isdesliavo savo laivus." << endl;
    }

    void rodytiLentas() const {
        cout << endl << "--- Jusu Lenta ---" << endl;
        zmogus.gautiLenta().atspausdinti(false);
        cout << endl << "--- Priesininko Lenta ---" << endl;
        kompiuteris.gautiLenta().atspausdinti(true);
    }

    void zaistiZaidima() {
        while (true) {
            rodytiLentas();

            cout << endl << "-- Jusu eile --" << endl;
            Pozicija suvimas;
            string rezultatas;

            do {
                suvimas = zmogus.gautiSuvima();
                rezultatas = kompiuteris.gautiLenta().gautiSuvima(suvimas.eilute, suvimas.stulpelis);
                if (rezultatas == "jau")
                    cout << "Jus jau saudet ten! Bandykite dar karta." << endl;
            } while (rezultatas == "jau");

            if (rezultatas == "praleista") {
                cout << "Praleista!" << endl;
            }
            else if (rezultatas == "pataikyta") {
                cout << "Pataikyta!" << endl;
            }
            else {
                cout << "Jusu nusmusite " << rezultatas.substr(7) << "!" << endl;
            }

            if (kompiuteris.gautiLenta().visiNumusti()) {
                rodytiLentas();
                cout << endl << "*** Jusu pergale! Visi priesinink? laivai nuskandinti! ***" << endl;
                break;
            }

            cout << endl << "-- Kompiuterio eile --" << endl;
            do {
                suvimas = kompiuteris.gautiSuvima();
                rezultatas = zmogus.gautiLenta().gautiSuvima(suvimas.eilute, suvimas.stulpelis);
            } while (rezultatas == "jau");

            cout << "Kompiuteris saudo i (" << suvimas.eilute << "," << suvimas.stulpelis << "): ";

            if (rezultatas == "praleista") {
                cout << "Praleista!" << endl;
            }
            else if (rezultatas == "pataikyta") {
                cout << "Pataikyta!" << endl;
            }
            else {
                cout << "Nuskandino jusu " << rezultatas.substr(7) << "!" << endl;
            }

            if (zmogus.gautiLenta().visiNumusti()) {
                rodytiLentas();
                cout << endl << "*** Kompiuteris laimejo! Visi jusu laivai nuskandinti. ***" << endl;
                break;
            }
        }
    }

    void paleisti() {
        srand(static_cast<unsigned>(time(nullptr)));
        paruosti();
        zaistiZaidima();
    }
};

int main() {
    Zaidimas zaidimas;
    zaidimas.paleisti();
    return 0;
}
