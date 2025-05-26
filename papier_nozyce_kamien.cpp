#include <iostream>
#include <string>
int g, k, pg, pk, grasz_dalej;

std::string f0(int numer){
    if (numer == 1){
        return "papier";
    }
    if (numer == 2){
        return "nożyce";
    }
    if (numer == 3){
        return "kamień";
    }
}
int f1() {
    int grasz_dalej;
    pk, pg = 0; //zerowanie punktów
    while (pk < 3 && pg < 3){
        std::cout << "Aktualna punktacja: " << pg << ":" << pk << std::endl;
        std::cout << "Twój ruch! (1: papier, 2: nożyce, 3:kamień) : ";
        std::cin >> g;
        k = (rand() % 3) + 1; //losuje wybór komputera
        std::cout << "Ty : " << f0(g) << ", Komputer : " << f0(k) << " ... " << std::endl;
        
        if((g==2 && k == 1) || (g==3 && k == 2) || (g==1 && k == 3)){
            pg++;
            std::cout << "Punkt dla Ciebie!"  << std::endl;
        }
        if((k==2 && g == 1) || (k==3 && g == 2) || (k==1 && g == 3)){
            pk++;
            std::cout << "Punkt dla Komputera!" << std::endl;
        }
        if((g==1 && k == 1) || (g==2 && k == 2) || (g==3 && k == 3)){
            std::cout << "Remis!" << std::endl;
        }
    }
    if (pk == 3){
        std::cout << "Nie udało się... Komputer wygrwał... " << std::endl;
    } else {
        std::cout << "Brawo! Wygrałeś!" << std::endl;
    }
    std::cout << "Grasz dalej? (1: TAK, 0: NIE) : ";
    std::cin >> grasz_dalej;
    return grasz_dalej;
}

int main() {
    srand(time(0)); // Inicjalizacja generatora liczb losowych
    while (f1() == 1) {
        std::cout << "Gramy dalej!" << std::endl;
    }

    std::cout << "Dziękuję! Do następnego!" << std::endl;
    return 0;
}
