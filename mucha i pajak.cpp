#include <stdio.h>
#include <vector>
#include <utility>
#include <iostream>
#include <string>

std::pair<int, int> mucha, pajak;
int wielkosc_planszy;

void ustaw_muche_na_srodku()
{
    mucha.first = mucha.second = ((wielkosc_planszy)/2) +1;
}

void losuj_poczatek_pajaka()
{
    int i;
    std::vector<std::pair<int, int>> vec; // Deklaracja pustego wektora par liczb całkowitych
    std::pair<int, int> nthPair;
    for (i=1; i<=wielkosc_planszy; i++){ 
        //np. wynik dla wielkosc_planszy=5: (1,1) i (1,5), potem (2,1) i (2,5), potem (3,1) i (3,5) itd
        vec.push_back({i, 1}); 
        vec.push_back({i, wielkosc_planszy});
    }
    for(i=2; i<wielkosc_planszy; i++){
        //np. wynik dla wielkosc_planszy=5: (1,2) i (5,2), potem (1,3) i (5,3), potem (1,4) i (5,4) i koniec
        vec.push_back({1, i});
        vec.push_back({wielkosc_planszy, i});
    }
    //sprawdzenie: for(i=0; i<vec.size(); i++){std::cout << "(" << vec[i].first << "," << vec[i].second << "),";}
    //losowanie pozycji początkowej pajaka
    i = rand() % wielkosc_planszy;
    //sprawdzenie: std::cout << "element: " << i << ", P(x, y) = (" << vec[i].first << ", " << vec[i].second << ")" <<  std::endl;
    //przypisanie wylosowanej pozycji pająkowi
    pajak.first = vec[i].first;
    pajak.second = vec[i].second;
}

void pokazPlansze(){
    int x, y;
        std::wstring MessagWithWebdings = L"test \u00A3";
    for (y=wielkosc_planszy; y>=1; y--){
        for (x=1; x<=wielkosc_planszy; x++){
            if (x==mucha.first && y==mucha.second){
                std::cout << "M" << " ";
            } 
            else{
                if (x==pajak.first && y==pajak.second){
                    std::cout << "P" << " ";
                } 
                else {
                    std::cout << "\u25A1" << " ";
                }
            }
        }
        std::cout << std::endl;
    }
}

int przesun(std::pair<int, int>& kogo, int kierunek){
//ustalilismy miedzy soba, ze 0 = gora, 1 = dol, 2 = lewo, 3 = prawo
//jesli ruch jest poprawny/mozliwy to zwroc 1, w przeciwnym wypadku zwroc 0 (np. stoisz na samej gorze a chcesz pojsc do gory - nic nie przesunie i zwroci 0)
int ruch_poprawny = 0;
if(kierunek==0 && kogo.second < wielkosc_planszy){
    kogo.second++;
    ruch_poprawny = 1;
}
if(kierunek==1 && kogo.second > 1){
    kogo.second--;
    ruch_poprawny = 1;
}
if(kierunek==2 && kogo.first > 1){
    kogo.first--;
    ruch_poprawny = 1;
}
if(kierunek==3 && kogo.first < wielkosc_planszy){
    
    kogo.first++;
    ruch_poprawny = 1;
}
return ruch_poprawny;
}

int ruch_pajaka(){
    //losuj ruch pajaka
    int k = rand() % 4;
    przesun(pajak, k);
    return -1;
}

int ruch_muchy(){
 std::string klick;
 std::cout << "wybierz ruch muchy (w,a,s,d):";
 int kier;
    std::cin >> klick;
    if (klick=="w"){
        kier=0;
    }
    if (klick=="s"){
        kier=1;
    }
    if (klick=="a"){
        kier=2;
    }
    if (klick=="d"){
        kier=3;
    }
    
    przesun(mucha,kier);
    return -1;
}

bool kolizja(){
    if(mucha.first==pajak.first && mucha.second==pajak.second){
        return true;
    } 
        else {
            return false;
        }    
}

int main()
{  
int licznik_ruchow=0;

    bool czy_kolizja = false;
    srand(time(0)); // Inicjalizacja generatora liczb losowych
    std::cout << "Jaka wielkość planszy (3,5,7): ";
    std::cin >> wielkosc_planszy;
    losuj_poczatek_pajaka();
    ustaw_muche_na_srodku();
    pokazPlansze();
     while (czy_kolizja == false && licznik_ruchow < 10)
    {
        licznik_ruchow++;
        
        ruch_muchy();
        
        czy_kolizja = kolizja();
        if(czy_kolizja == false){
            ruch_pajaka();
        }
        else{
            std::cout << std::endl << "Przegrałeś. Pająk robi om nom nom." << std::endl;    
        }
        czy_kolizja = kolizja();
        if (czy_kolizja) {
            std::cout << std::endl << "Przegrałeś. Pająk robi om nom nom." << std::endl;
        }
        pokazPlansze();
    }
    if (czy_kolizja == false){
        std::cout << std::endl << "Hura!!!  Mucha przyzyla" << std::endl;
    }
    return 0;
}
