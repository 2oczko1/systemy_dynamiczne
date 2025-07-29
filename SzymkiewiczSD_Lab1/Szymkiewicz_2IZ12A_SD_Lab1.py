import numpy as np
import msvcrt

def kryterium_hurwitza(wspolczynniki):
    n = len(wspolczynniki)
    if n == 3:
        # Obliczenie minora głównego wyznacznika macierzy Hurwitza
        D1 = wspolczynniki[0]
        D2 = int(np.linalg.det(np.array([[wspolczynniki[0], wspolczynniki[2]], [1, wspolczynniki[1]]])))
        
        # Sprawdzenie warunków na stabilność
        if D1 > 0 and wspolczynniki[1] > 0 and D2 > 0:
            return "stabilny asymptotycznie"
        elif D2 == 0:
            return "na granicy stabilności"
        else:
            return "niestabilny"
    else:
        return "Niepoprawna długość wektora współczynników"

def main():
    # Wprowadzenie współczynników transmitancji przez użytkownika
    print("Podaj współczynniki transmitancji G(s):")
    a0 = int(input("Podaj współczynnik a0: "))
    a1 = int(input("Podaj współczynnik a1: "))
    a2 = int(input("Podaj współczynnik a2: "))

    # Obliczenie współczynników macierzy Hurwitza
    D1 = a1
    D2 = int(np.linalg.det(np.array([[a1, a2], [0, a0]])))
    print("Współczynniki macierzy Hurwitza:")
    print("D1 =", D1)
    print("D2 =", D2)
    
    # Analiza stabilności za pomocą kryterium Hurwitza
    wynik = kryterium_hurwitza([D1, D2, a0])
    print("Wynik analizy stabilności układu:", wynik)

    # Oczekiwanie na naciśnięcie dowolnego klawisza przez użytkownika
    print("Naciśnij dowolny przycisk, aby zakończyć...")
    msvcrt.getch()

if __name__ == "__main__":
    main()
