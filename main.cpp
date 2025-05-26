#include <stdio.h>
#include <vector>
#include <utility>
#include <iostream>

std::pair<int, int> mucha, pajak;
int wielkosc_planszy;

int main()
{
    srand(time(0)); // Inicjalizacja generatora liczb losowych
    std::cout << "Jaka wielkość planszy (3,5,7): ";
    std::cin >> wielkosc_planszy;

    return 0;
}

int losuj_poczatek_pajaka()
{
    int i;
    //float xsr=static_cast<float>(5)/2;
    std::vector<std::pair<int, int>> vec; // Deklaracja pustego wektora par liczb całkowitych
    std::pair<int, int> nthPair;
    for (i=0; i<wielkosc_planszy; i++){
        vec.push_back({i, 1});
        vec.push_back({i, wielkosc_planszy});
    }
}
