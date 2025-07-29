#include <iostream>   // Artur Szymkiewicz, Michal Wojtachnio
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

// Struktura do przechowywania punktow na sciezce
struct Point {
    int x;
    int y;
};

// Funkcja pomocnicza do wyswietlania siatki z nazwami punktow
void printGridWithNames(int rows, int cols) {
    cout << "Graficzne przedstawienie siatki:\n";
    char letter = 'A';
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << letter << (j + 1) << " ";
        }
        letter++;
        cout << endl;
    }
}

// Funkcja pomocnicza do wyswietlania siatki z wartosciami
void printGrid(const vector<vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int val : row) {
            cout << setw(2) << val << " ";
        }
        cout << endl;
    }
}

// Funkcja pomocnicza do wyswietlania trajektorii na siatce wraz z kosztami
void printPath(const vector<vector<int>>& grid, const vector<Point>& path) {
    vector<vector<char>> pathGrid(grid.size(), vector<char>(grid[0].size(), ' '));
    for (const auto& point : path) {
        pathGrid[point.x][point.y] = '*';
    }
    pathGrid[grid.size() - 1][0] = 'P'; // POCZATEK
    pathGrid[0][grid[0].size() - 1] = 'K'; //KONIEC

    int totalCost = 0;
    cout << "Koszty przejscia:\n";
    for (size_t i = 0; i < path.size(); ++i) {
        totalCost += grid[path[i].x][path[i].y];
        char letter = 'A' + path[i].x;
        cout << letter << (path[i].y + 1) << " (" << grid[path[i].x][path[i].y] << ")";
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << "\nSuma kosztow: " << totalCost << endl;

    for (const auto& row : pathGrid) {
        for (char c : row) {
            cout << c << " ";
        }
        cout << endl;
    }
}

// Rekurencyjna funkcja do znajdowania wszystkich mozliwych sciezek
void findPaths(const vector<vector<int>>& grid, int x, int y, vector<Point>& currentPath, int currentCost, vector<vector<Point>>& allPaths, vector<int>& allCosts) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Jesli dotarlismy do prawego gornego rogu, zapisz sciezke i koszt
    if (x == 0 && y == cols - 1) {
        currentPath.push_back({ x, y });
        allPaths.push_back(currentPath);
        allCosts.push_back(currentCost + grid[x][y]);
        currentPath.pop_back();
        return;
    }

    // Dodaj aktualny punkt do sciezki
    currentPath.push_back({ x, y });

    // Przemieszczamy sie w gore
    if (x > 0) {
        findPaths(grid, x - 1, y, currentPath, currentCost + grid[x][y], allPaths, allCosts);
    }
    // Przemieszczamy sie w prawo
    if (y < cols - 1) {
        findPaths(grid, x, y + 1, currentPath, currentCost + grid[x][y], allPaths, allCosts);
    }

    // Usuwamy aktualny punkt ze sciezki (backtracking)
    currentPath.pop_back();
}

int main() {
    int rows, cols;

    while (true) {
        cout << "Podaj liczbe wierszy i kolumn siatki: ";
        if (cin >> rows >> cols && rows > 0 && cols > 0) {
            break;
        }
        else {
            cout << "Niepoprawna wartosc! Przyklad poprawnego wprowadzenia: 4 5" << endl;
            cin.clear(); // Wyczysc stan bledu strumienia wejsciowego
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Wyczysc bufor wejsciowy
        }
    }

    // Wyswietlenie siatki z nazwami punktow
    printGridWithNames(rows, cols);

    vector<vector<int>> grid(rows, vector<int>(cols));
    cout << "Podaj wartosci dla siatki punktow (tylko liczby calkowite):\n";
    for (int i = 0; i < rows; ++i) {
        char letter = 'A' + i;
        for (int j = 0; j < cols; ++j) {
            while (true) {
                cout << "Podaj wartosc dla punktu " << letter << (j + 1) << ": ";
                if (cin >> grid[i][j]) {
                    break;
                }
                else {
                    cout << "Niepoprawna wartosc! Wprowadz liczbe calkowita." << endl;
                    cin.clear(); // Wyczysc stan bledu strumienia wejsciowego
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Wyczysc bufor wejsciowy
                }
            }
        }
    }

    // Inicjalizacja zmiennych do przechowywania wszystkich mozliwych sciezek i ich kosztow
    vector<vector<Point>> allPaths;
    vector<int> allCosts;
    vector<Point> currentPath;

    // Znajdowanie wszystkich mozliwych sciezek
    findPaths(grid, rows - 1, 0, currentPath, 0, allPaths, allCosts);

    // Wyswietlenie wszystkich sciezek i wartosci wskaznika jakosci
    cout << "\nWszystkie mozliwe sciezki i wartosci wskaznika jakosci:\n";
    for (size_t i = 0; i < allPaths.size(); ++i) {
        cout << "sciezka " << i + 1 << " (koszt: " << allCosts[i] << "):\n";
        for (const auto& point : allPaths[i]) {
            char letter = 'A' + point.x;
            cout << letter << (point.y + 1) << " ";
        }
        cout << endl;
        printPath(grid, allPaths[i]);
        cout << endl;
    }

    // Wybranie najlepszej sciezki (o najmniejszym koszcie)
    int minCostIndex = 0;
    for (size_t i = 1; i < allCosts.size(); ++i) {
        if (allCosts[i] < allCosts[minCostIndex]) {
            minCostIndex = i;
        }
    }

    // Wyswietlenie najlepszej sciezki
    cout << "\nNajlepsza sciezka (o najmniejszym koszcie " << allCosts[minCostIndex] << "):\n";
    for (const auto& point : allPaths[minCostIndex]) {
        char letter = 'A' + point.x;
        cout << letter << (point.y + 1) << " ";
    }
    cout << endl;

    // Wyswietlenie graficznego przedstawienia siatki z najlepsza trasa
    cout << "\nGraficzne przedstawienie siatki z najlepsza trasa:\n";
    printPath(grid, allPaths[minCostIndex]);

    // Czekanie na zamkniecie przez uzytkownika
    cout << "Nacisnij klawisz ENTER, aby zakończyc..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Wyczyść bufor wejściowy, jeśli są jakieś znaki w buforze
    cin.get();

    return 0;
}
