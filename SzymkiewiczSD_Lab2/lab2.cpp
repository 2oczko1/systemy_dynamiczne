#include <iostream>   Artur Szymkiewicz, Michal Wojtachnio
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <locale>
#include <limits>

// Definicje zakresow dla parametrow genetycznych
const double MIN_X = 0.0;
const double MAX_X = 10.0;
const int ROZMIAR_POPULACJI = 100;
const int MAX_GENERACJI = 100;
const double WSZ_MUTACJI = 0.1;

// Struktura reprezentujaca transmitancje operatorowa
struct TransmitancjaOperatorowa {
    double K; // Wspolczynnik wzmocnienia statycznego
    double T; // Stala czasowa
    double xi; // Wspolczynnik tlumienia
};

// Funkcja obliczajaca dopasowanie (fitness)
double dopasowanie(const TransmitancjaOperatorowa& tf, const std::vector<double>& pomiary) {
    double blad = 0.0;
    // Obliczanie bledu dla kazdego pomiaru
    for (size_t i = 0; i < pomiary.size(); ++i) {
        // Sprawdzenie, czy parametry tf.T i tf.xi sa wieksze od zera i tf.xi jest mniejsze niz 1
        if (tf.T > 0 && tf.xi > 0 && tf.xi < 1) {
            // Obliczenia sa bezpieczne do wykonania
            // ...
        }
        else {
            // Jesli ktores z tych parametrow jest nieprawidlowe, zwracamy wartosc nieskończonosci
            return std::numeric_limits<double>::infinity();
        }
    }
    return blad;
}


// Funkcja do pobierania parametrow od uzytkownika
void pobierzParametry(int& przestrzeńPoszukiwań, int& liczbaIteracji, int& rozmiarPopulacji, double& prawdopodobieństwoKrzyzowania, double& prawdopodobieństwoMutacji) {
    std::cout << "Podaj przestrzen poszukiwan (liczba calkowita): ";
    std::cin >> przestrzeńPoszukiwań;

    std::cout << "Podaj liczbe iteracji (liczba calkowita): ";
    std::cin >> liczbaIteracji;

    std::cout << "Podaj rozmiar populacji (liczba calkowita): ";
    std::cin >> rozmiarPopulacji;

    std::cout << "Podaj prawdopodobienstwo krzyzowania (0-1): ";
    std::cin >> prawdopodobieństwoKrzyzowania;

    std::cout << "Podaj prawdopodobienstwo mutacji (0-1): ";
    std::cin >> prawdopodobieństwoMutacji;
}

// Tworzenie poczatkowej populacji
std::vector<TransmitancjaOperatorowa> stworzPoczatkowaPopulacje(int rozmiarPopulacji) {
    std::vector<TransmitancjaOperatorowa> populacja;
    for (int i = 0; i < rozmiarPopulacji; ++i) {
        TransmitancjaOperatorowa to;
        to.K = MIN_X + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (MAX_X - MIN_X)));
        to.T = MIN_X + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (MAX_X - MIN_X)));
        to.xi = MIN_X + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (MAX_X - MIN_X)));
        populacja.push_back(to);
    }
    return populacja;
}

// Selekcja turniejowa
TransmitancjaOperatorowa selekcjaTurniejowa(const std::vector<TransmitancjaOperatorowa>& populacja, const std::vector<double>& pomiary) {
    // Losowy wybor dwoch osobnikow
    int idx1 = rand() % populacja.size();
    int idx2 = rand() % populacja.size();
    // Wybor osobnika o mniejszym bledzie
    if (dopasowanie(populacja[idx1], pomiary) < dopasowanie(populacja[idx2], pomiary)) {
        return populacja[idx1];
    }
    else {
        return populacja[idx2];
    }
}

// Krzyzowanie
TransmitancjaOperatorowa krzyzowanie(const TransmitancjaOperatorowa& rodzic1, const TransmitancjaOperatorowa& rodzic2, double prawdopodobieństwoKrzyzowania) {
    TransmitancjaOperatorowa dziecko = rodzic1;
    // Sprawdzenie czy krzyzowac
    if (static_cast<double>(rand()) / RAND_MAX < prawdopodobieństwoKrzyzowania) {
        // Punkt krzyzowania
        double punktKrzyzowania = static_cast<double>(rand()) / RAND_MAX;
        dziecko.K = punktKrzyzowania * rodzic1.K + (1 - punktKrzyzowania) * rodzic2.K;
        dziecko.T = punktKrzyzowania * rodzic1.T + (1 - punktKrzyzowania) * rodzic2.T;
        dziecko.xi = punktKrzyzowania * rodzic1.xi + (1 - punktKrzyzowania) * rodzic2.xi;
    }
    return dziecko;
}

// Mutacja
void mutacja(TransmitancjaOperatorowa& to, double prawdopodobieństwoMutacji) {
    // Mutacja polega na dodaniu losowej wartosci do kazdego parametru
    if (static_cast<double>(rand()) / RAND_MAX < prawdopodobieństwoMutacji) {
        to.K += (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
    }
    if (static_cast<double>(rand()) / RAND_MAX < prawdopodobieństwoMutacji) {
        to.T += (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
    }
    if (static_cast<double>(rand()) / RAND_MAX < prawdopodobieństwoMutacji) {
        to.xi += (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
    }
}
int main() {
    // Inicjalizacja generatora liczb losowych
    srand(static_cast<unsigned int>(time(nullptr)));

    // Przykladowe dane pomiarowe
    std::vector<double> pomiary = { 1.0, 2.0, 3.0, 4.0, 5.0 };

    // Parametry kontrolne
    int przestrzeńPoszukiwań, liczbaIteracji, rozmiarPopulacji;
    double prawdopodobieństwoKrzyzowania, prawdopodobieństwoMutacji;

    // Pobranie parametrow od uzytkownika
    pobierzParametry(przestrzeńPoszukiwań, liczbaIteracji, rozmiarPopulacji, prawdopodobieństwoKrzyzowania, prawdopodobieństwoMutacji);

    // Inicjalizacja populacji poczatkowej
    std::vector<TransmitancjaOperatorowa> populacja = stworzPoczatkowaPopulacje(rozmiarPopulacji);

    // Inicjalizacja najlepszego osobnika
    TransmitancjaOperatorowa najlepszy_osobnik = populacja[0];
    double najlepsze_dopasowanie = dopasowanie(populacja[0], pomiary);

    // Petla glowna algorytmu genetycznego
    for (int iteracja = 0; iteracja < liczbaIteracji; ++iteracja) {
        // Selekcja, krzyzowanie i mutacja
        std::vector<TransmitancjaOperatorowa> nastepna_generacja;
        for (int i = 0; i < rozmiarPopulacji; ++i) {
            TransmitancjaOperatorowa rodzic1 = selekcjaTurniejowa(populacja, pomiary);
            TransmitancjaOperatorowa rodzic2 = selekcjaTurniejowa(populacja, pomiary);
            TransmitancjaOperatorowa potomek = krzyzowanie(rodzic1, rodzic2, prawdopodobieństwoKrzyzowania);
            mutacja(potomek, prawdopodobieństwoMutacji);
            nastepna_generacja.push_back(potomek);
        }
        // Aktualizacja populacji
        populacja = nastepna_generacja;

        // Wyszukanie najlepszego osobnika w populacji
        for (const auto& osobnik : populacja) {
            double dopasowanie_osobnika = dopasowanie(osobnik, pomiary);
            if (dopasowanie_osobnika < najlepsze_dopasowanie) {
                najlepsze_dopasowanie = dopasowanie_osobnika;
                najlepszy_osobnik = osobnik;
            }
        }

        // Wyswietlenie najlepszego osobnika w danej iteracji
        std::cout << "Iteracja " << iteracja + 1 << ": Najlepsze dopasowanie = " << najlepsze_dopasowanie << std::endl;
        std::cout << "Najlepszy osobnik: K = " << najlepszy_osobnik.K << ", T = " << najlepszy_osobnik.T << ", J = " << najlepszy_osobnik.xi << std::endl;
    }

    // Wyswietlenie najlepszego osobnika ze wszystkich iteracji
    std::cout << "Najlepszy osobnik ze wszystkich iteracji: K = " << najlepszy_osobnik.K << ", T = " << najlepszy_osobnik.T << ", J = " << najlepszy_osobnik.xi << std::endl;

    return 0;
}