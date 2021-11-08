#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <stdarg.h>

using namespace std;

class Wierzcholek
{
public:
    int id;
    int stopien = 0;
};

class Krawedz
{
    public:
    int waga;
    Wierzcholek lewy;
    Wierzcholek prawy;
    float ilosc_przejsc_krawedzi;
};

struct Wierzcholek_Krawedz
{
    int wierzcholek_id;
    int krawedz_id;
};

struct Instancja
{
    vector <int> wierzcholki_id;
    vector <int> krawedzie_id;
};

int ilosc_wierzcholkow;
vector <Wierzcholek> lista_wierzcholkow;
vector <Krawedz> lista_krawedzi;
vector <Wierzcholek> sprawdzone;

double losuj_od_zero_do_jeden()
{
    const int N = 10000;
    vector<double> permutation(N);
    for (int i = 0; i < N; i++)
        permutation[i] = i;

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    shuffle(permutation.begin(), permutation.end(), rng);

    return permutation[0] / N;
}

void utworz_wierzcholki()
{

    lista_wierzcholkow.clear();
    for(int i=1; i <= ilosc_wierzcholkow; i++)
    {
        Wierzcholek pierwszy;
        pierwszy.id = i;
        lista_wierzcholkow.push_back(pierwszy);
    }
}

void wyczysc_stopnie()
{
    for(int i=0; i<lista_wierzcholkow.size(); i++)
    {
        lista_wierzcholkow[i].stopien = 0;
    }
}

void utworz_krawedzie(int ilosc_krawedzi, int min_stopien, int max_stopien)
{
    wyczysc_stopnie();

    lista_krawedzi.clear();

    bool zrobil_sie = false;

    for(int i=0; i<ilosc_krawedzi; i++)
    {
       Krawedz pierwsza;

       int losowa_zmienna1;
       int losowa_zmienna2;

       int wierzcholki_o_stopniu_innym_niz_max = 0;
       for(int j=0; j<lista_wierzcholkow.size(); j++)
       {
           if(lista_wierzcholkow[j].stopien < max_stopien)
           {
               wierzcholki_o_stopniu_innym_niz_max++;
           }
       }

       if(wierzcholki_o_stopniu_innym_niz_max == 1)
       {
           utworz_krawedzie(ilosc_krawedzi, min_stopien, max_stopien);
           return;
       }

        do
        {
            losowa_zmienna1 = rand() % ilosc_wierzcholkow + 0;
        }
        while(lista_wierzcholkow[losowa_zmienna1].stopien == max_stopien);

        do
        {
            losowa_zmienna2 = rand() % ilosc_wierzcholkow + 0;
        }
        while(losowa_zmienna1 == losowa_zmienna2 || lista_wierzcholkow[losowa_zmienna2].stopien == max_stopien);

        lista_wierzcholkow[losowa_zmienna1].stopien++; // do liczenia stopnia
        lista_wierzcholkow[losowa_zmienna2].stopien++;

        pierwsza.lewy = lista_wierzcholkow[losowa_zmienna1];
        pierwsza.prawy = lista_wierzcholkow[losowa_zmienna2];


        int losowa_waga = rand() % 100 + 1;

        pierwsza.waga = losowa_waga;
        lista_krawedzi.push_back(pierwsza); // cala krawedz
    }
    for(int i = 0; i < lista_wierzcholkow.size(); i++)
    {
        if (lista_wierzcholkow[i].stopien < min_stopien)
        {
            utworz_krawedzie(ilosc_krawedzi, min_stopien, max_stopien);
            return;
        }
    }
}

bool czy_wierzcholek_nie_byl_sprawdzony(Wierzcholek w)
{
    for(int i=0; i< sprawdzone.size(); i++)
    {
        if(w.id == sprawdzone[i].id)
        {
            return false;
        }
    }
    return true;
}

bool czy_jest_jedna_skladowa(Wierzcholek w)   // czy jest spojny, obiekt w w klasy wierzcholek
{

    sprawdzone.push_back(w);
    for(int i=0; i<lista_krawedzi.size(); i++)
    {
        if(lista_krawedzi[i].lewy.id == w.id && czy_wierzcholek_nie_byl_sprawdzony(lista_krawedzi[i].prawy))
        {
            czy_jest_jedna_skladowa(lista_krawedzi[i].prawy);
        }
         if(lista_krawedzi[i].prawy.id == w.id && czy_wierzcholek_nie_byl_sprawdzony(lista_krawedzi[i].lewy))
        {
            czy_jest_jedna_skladowa(lista_krawedzi[i].lewy);
        }
    }
      if(sprawdzone.size() == ilosc_wierzcholkow)
      {
          return true;
      }
      else
      {
          return false;
      }
}

void wyswietl_graf()
{
    cout << "Lewy-ID, Prawy-ID, Ilosc przejsc krawedzi" << endl;
    for(int i=0; i<lista_krawedzi.size(); i++)
    {
        printf("%3d %3d %8.2f\n", lista_krawedzi[i].lewy.id, lista_krawedzi[i].prawy.id, lista_krawedzi[i].ilosc_przejsc_krawedzi);
        //cout << lista_krawedzi[i].lewy.id << " " <<lista_krawedzi[i].prawy.id << " " << lista_krawedzi[i].ilosc_przejsc_krawedzi << endl;
    }
}

void generuj_graf(int wierzcholki, float stopien_min, float stopien_max)
{
    ilosc_wierzcholkow = wierzcholki;
    int N_min;
    if (stopien_min==1)
    {
        N_min = ilosc_wierzcholkow-1;
    }
    else
    {
        N_min = (stopien_min/2)*ilosc_wierzcholkow;
    }
// rand
    int licznik = 0;
    int N_max = (stopien_max/2)*ilosc_wierzcholkow;             //stopien/2, bo krawedz idzie do 2 wierzcholkow
    vector<int> permutation(N_max-N_min+1);    //rozmiar: od max odejmujemy min (wierzcholki-1) +1
    for (int i = N_min; i <= N_max; i++)     //min = wierzcholki - 1, bo krawedzi moze byc minimalnie o 1 mniej niz wierzcholkow
    {
        permutation[licznik++] = i;                     //zewnetrzny licznik bo i nie jest od 0
    }
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    int ilosc_krawedzi = 0;
    do
    {
        shuffle(permutation.begin(), permutation.end(), rng);
        ilosc_krawedzi = permutation[0];
        sprawdzone.clear();
        utworz_wierzcholki();
        utworz_krawedzie(ilosc_krawedzi, stopien_min, stopien_max);
    }
    while(!czy_jest_jedna_skladowa(lista_wierzcholkow[0]));

    cout << "Nowy graf\nIlosc wierzcholkow: " << ilosc_wierzcholkow <<"\nIlosc krawedzi:" << ilosc_krawedzi << endl;
}

Wierzcholek_Krawedz wybierz_wierzcholek(int obecny_wierzcholek, bool losowo) //jesli nie losowo, to macierzowo
{
    vector <int> tymczasowi_sasiedzi;
    vector <int> tymczasowe_krawedzie;

	for(int x=0; x<lista_krawedzi.size(); x++)
	{
		if(lista_krawedzi[x].prawy.id == obecny_wierzcholek)
		{
			tymczasowi_sasiedzi.push_back(lista_krawedzi[x].lewy.id);
			tymczasowe_krawedzie.push_back(x);
		}
		if(lista_krawedzi[x].lewy.id == obecny_wierzcholek)
		{
			tymczasowi_sasiedzi.push_back(lista_krawedzi[x].prawy.id);
			tymczasowe_krawedzie.push_back(x);
		}
	}

	if(losowo)
	{
		int pozycja_obecnego_wierzcholka = rand() % tymczasowi_sasiedzi.size() + 0; // losujemy pozycje z wektora

		return {tymczasowi_sasiedzi[pozycja_obecnego_wierzcholka], tymczasowe_krawedzie[pozycja_obecnego_wierzcholka]};
	}
	else //macierzowo
	{
		float suma_przejsc_sasiadow = 0;
		for(int i = 0; i<tymczasowe_krawedzie.size(); i++)
		{
			suma_przejsc_sasiadow += lista_krawedzi[tymczasowe_krawedzie[i]].ilosc_przejsc_krawedzi;
		}

		if (suma_przejsc_sasiadow == 0)
        {
            int pozycja_obecnego_wierzcholka = rand() % tymczasowi_sasiedzi.size() + 0; // losujemy pozycje z wektora

            return {tymczasowi_sasiedzi[pozycja_obecnego_wierzcholka], tymczasowe_krawedzie[pozycja_obecnego_wierzcholka]};
		}

        //sortowanie
		for(int j=0; j<tymczasowe_krawedzie.size() - 1; j++)
		{
			for(int i=0; i<tymczasowe_krawedzie.size() - 1 - j; i++)
			{
				if(lista_krawedzi[tymczasowe_krawedzie[i]].ilosc_przejsc_krawedzi < lista_krawedzi[tymczasowe_krawedzie[i+1]].ilosc_przejsc_krawedzi)
				{
					int temp = tymczasowe_krawedzie[i] ;
					tymczasowe_krawedzie[i] = tymczasowe_krawedzie[i+1];
					tymczasowe_krawedzie[i+1] = temp;

					int temp2 = tymczasowi_sasiedzi[i] ;
					tymczasowi_sasiedzi[i] = tymczasowi_sasiedzi[i+1];
					tymczasowi_sasiedzi[i+1] = temp2;
				}
			}
		}

		float wartosc_progowa = losuj_od_zero_do_jeden();
		float tymczasowa_suma = 0;

		for(int i=0; i<tymczasowi_sasiedzi.size(); i++)
		{
			tymczasowa_suma += lista_krawedzi[tymczasowe_krawedzie[i]].ilosc_przejsc_krawedzi / suma_przejsc_sasiadow;
			if(wartosc_progowa <= tymczasowa_suma)
			{
				return {tymczasowi_sasiedzi[i], tymczasowe_krawedzie[i]};
			}
		}
	}
}

Instancja losuj_instancje(float prawdopodobienstwo_uzycia_macierzy) //jesli prawdopodobienstwo jest wieksze niz wylosowana wartosc, to wybiera wierzcholek macierzowo, a jesli mniejsze (w tym ujemne) to losowo
{
    int obecny_wierzcholek = rand() % ilosc_wierzcholkow + 1 ;
    vector <int> lista_wierzcholkow_odwiedzonych;
    vector <int> lista_krawedzi_odwiedzonych;
    lista_wierzcholkow_odwiedzonych.push_back(obecny_wierzcholek);

    vector <int> v2; // to jest do kopii sortowania
    do
    {
        Wierzcholek_Krawedz wierzch_kraw;
        if(losuj_od_zero_do_jeden() <= prawdopodobienstwo_uzycia_macierzy)
        {
            wierzch_kraw = wybierz_wierzcholek(obecny_wierzcholek, false);   //macierzowo
        }
        else
        {
            wierzch_kraw = wybierz_wierzcholek(obecny_wierzcholek, true);    //losowo
        }

        obecny_wierzcholek = wierzch_kraw.wierzcholek_id;

        lista_wierzcholkow_odwiedzonych.push_back(obecny_wierzcholek);
        lista_krawedzi_odwiedzonych.push_back(wierzch_kraw.krawedz_id);

        //tworze wektor o unikalnych wierzcholkach
        v2 = lista_wierzcholkow_odwiedzonych; //kopia

        sort(v2.begin(), v2.end());
        vector<int>::iterator it;
        it = unique(v2.begin(), v2.end());
        v2.resize(distance(v2.begin(),it));
    }while(v2.size() != lista_wierzcholkow.size()); //liczba unikalnych != wierzcholkom

    return {lista_wierzcholkow_odwiedzonych, lista_krawedzi_odwiedzonych};
}

float wiele_instancji(float prawdopodobienstwo, int ilosc_instancji) //jesli prawdopodobienstwo bedzie ujemne, to traktujemy to jako pierwsze 100 glupich mrowek. Inaczej to kolejne 100 madrych
{
    vector <vector<int>> lista_instancji_krawedzi;
    vector <vector<int>> lista_instancji_wierzcholkow;
    vector <int> lista_s;

    int suma_dlugosci_sciezek = 0;

    for(int i = 0; i<ilosc_instancji; i++)
    {
        Instancja instancja = losuj_instancje(prawdopodobienstwo);

        lista_instancji_wierzcholkow.push_back(instancja.wierzcholki_id);
        lista_instancji_krawedzi.push_back(instancja.krawedzie_id);

        int s = 0; // do liczenia tych 10 najlepszych
        int ilosc_przejsc_obecnej_krawedzi[instancja.krawedzie_id.size()];
        for (int j = 0; j< instancja.krawedzie_id.size(); j++)
        {
            ilosc_przejsc_obecnej_krawedzi[j] = 0;
        }

        for(int j=0; j<instancja.krawedzie_id.size(); j++)
        {
            ilosc_przejsc_obecnej_krawedzi[j]++;
            s = s + ilosc_przejsc_obecnej_krawedzi[j] * lista_krawedzi[instancja.krawedzie_id[j]].waga;
            suma_dlugosci_sciezek += lista_krawedzi[instancja.krawedzie_id[j]].waga;
        }
        lista_s.push_back(s);

    }
    for(float i=0;i<0.1*ilosc_instancji;i++)
    {
        int najmniejsza = 0;
        for(int j=1; j<lista_s.size();j++)
        {
            if(lista_s[najmniejsza]>lista_s[j])
            {
                najmniejsza = j;
            }
        }
       for(int j = 0; j<lista_instancji_krawedzi[najmniejsza].size();j++)
       {
           lista_krawedzi[lista_instancji_krawedzi[najmniejsza][j]].ilosc_przejsc_krawedzi += 1-(i/10);
       }
       lista_s.erase(lista_s.begin() + najmniejsza);
       lista_instancji_krawedzi.erase(lista_instancji_krawedzi.begin() + najmniejsza);
       lista_instancji_wierzcholkow.erase(lista_instancji_wierzcholkow.begin() + najmniejsza);
    }

    return suma_dlugosci_sciezek/(float)ilosc_instancji;
}

void parowanie_feromonow(float wartosc)
{
    for(int i = 0; i<lista_krawedzi.size(); i++)
    {
        float wartosc_macierzy = lista_krawedzi[i].ilosc_przejsc_krawedzi;
        wartosc_macierzy -= wartosc;
        if(wartosc_macierzy < 1)
        {
            wartosc_macierzy = 1;
        }
        lista_krawedzi[i].ilosc_przejsc_krawedzi = wartosc_macierzy;
    }
}

void wygladzanie_feromonow()
{
    vector <float> wektor_do_mediany;
    for (int i = 0; i < lista_krawedzi.size(); i++)
    {
        wektor_do_mediany.push_back(lista_krawedzi[i].ilosc_przejsc_krawedzi);
    }
    sort(wektor_do_mediany.begin(), wektor_do_mediany.end());
    float minimum; //mediana
    if (wektor_do_mediany.size() % 2 == 0)
    {
        minimum = (wektor_do_mediany[wektor_do_mediany.size()/2-1] + wektor_do_mediany[wektor_do_mediany.size()/2])/2;
    }
    else
    {
        minimum = wektor_do_mediany[wektor_do_mediany.size()/2];
    }
    for (int i = 0; i < lista_krawedzi.size(); i++)
    {
        float wartosc_macierzy = lista_krawedzi[i].ilosc_przejsc_krawedzi;
        float podstawa_logarytmu = 10;
        if (wartosc_macierzy>0)
        {
            wartosc_macierzy = wartosc_macierzy*0.75 + 0.25*(minimum * (1 + (log(wartosc_macierzy/minimum)/log(podstawa_logarytmu))));
        }
        lista_krawedzi[i].ilosc_przejsc_krawedzi = wartosc_macierzy;
    }
}

void test_czasu(int ilosc_testow, ...)
{
    cout << "--------------------\n     TEST CZASU     \n--------------------" << endl;
    va_list argumenty;
    va_start(argumenty, ilosc_testow);

    std::chrono::seconds duration;
    auto start = std::chrono::high_resolution_clock::now();

    int sekundy = va_arg(argumenty, int);
    cout << "\nTest " << sekundy << "-sekundowy (" << (float)sekundy/60 << "-minutowy):" << endl;
    float prawdopodobienstwo = 0;
    float srednia_dlugosc_sciezki;
    float najkrotsza_dlugosc = 0;
    bool chociaz_jeden_sie_zrobil = false;

    do
    {
        srednia_dlugosc_sciezki = wiele_instancji(prawdopodobienstwo, 100);
            //wyswietl_graf();
        auto finish = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::seconds>(finish - start);
        parowanie_feromonow(0.5);
            //wyswietl_graf();
        wygladzanie_feromonow();
            //wyswietl_graf();
        prawdopodobienstwo += 0.1;

        if (duration.count() < sekundy)
        {
            if (najkrotsza_dlugosc == 0 || najkrotsza_dlugosc > srednia_dlugosc_sciezki)
            {
                najkrotsza_dlugosc = srednia_dlugosc_sciezki;
            }
            cout << "Czas trwania: " << duration.count() << " sekund, Srednia dlugosc sciezki: " << srednia_dlugosc_sciezki << endl;
            //wyswietl_graf();
            chociaz_jeden_sie_zrobil = true;

        }
        else
        {
            ilosc_testow--;
            if (ilosc_testow > 0)
            {
                sekundy = va_arg(argumenty, int);
            }
            if(!chociaz_jeden_sie_zrobil)
            {
                cout << "Test nie zdazyl sie wykonac w zadanym czasie. Potrzebowal " << duration.count() << " sekund." << endl;
            }
            else
            {
                cout << "Najkrotsza sciezka dotychczas: " << najkrotsza_dlugosc << endl;
            }
            while (duration.count() >= sekundy && ilosc_testow > 0)
            {
                cout << "\nTest " << sekundy << "-sekundowy (" << (float)sekundy/60 << "-minutowy):" << endl;
                cout << "Test nie zdazyl sie wykonac w zadanym czasie. Potrzebowal " << duration.count() << " sekund." << endl;
                ilosc_testow--;
                if (ilosc_testow > 0)
                {
                    sekundy = va_arg(argumenty, int);
                }
            }
            if (duration.count() < sekundy && ilosc_testow > 0)
            {
                cout << "\nTest " << sekundy << "-sekundowy (" << (float)sekundy/60 << "-minutowy):" << endl;
                cout << "Czas trwania: " << duration.count() << " sekund, Srednia dlugosc sciezki: " << srednia_dlugosc_sciezki << endl;
                //wyswietl_graf();
                chociaz_jeden_sie_zrobil = true;
                if (najkrotsza_dlugosc == 0 || najkrotsza_dlugosc > srednia_dlugosc_sciezki)
                {
                    najkrotsza_dlugosc = srednia_dlugosc_sciezki;
                }
            }
        }

    }while (ilosc_testow > 0);

    //return najkrotsza_dlugosc;
}

void test_populacji(int czas)
{
        cout << "--------------------\n   TEST POPULACJI   \n--------------------" << endl;

    //generuj_graf(ilosc_wierzcholkow);
    std::chrono::seconds duration;

    float srednia_dlugosc_sciezki;

    const int ilosc_instancji_size = 5;
    int ilosc_instancji[] = {10,30,50,70,100};

    for (int i = 0; i < ilosc_instancji_size; i++)
    {
        for (int j = 0; j < lista_krawedzi.size(); j++)
        {
            lista_krawedzi[j].ilosc_przejsc_krawedzi = 0;
        }
        auto start = std::chrono::high_resolution_clock::now();
        float prawdopodobienstwo = 0;
        bool chociaz_jeden_sie_zrobil = false;
        cout << "\nTest dla populacji: " << ilosc_instancji[i] << endl;
        float najkrotsza_dlugosc = 0;
        do
        {
            srednia_dlugosc_sciezki = wiele_instancji(prawdopodobienstwo, ilosc_instancji[i]);
            parowanie_feromonow(0.5);
            wygladzanie_feromonow();
            prawdopodobienstwo+=0.1;
            auto finish = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::seconds>(finish - start);
            if (duration.count() < czas)
            {
                chociaz_jeden_sie_zrobil = true;
                cout << "Czas trwania: " << duration.count() << " sekund, Srednia dlugosc sciezki: " << srednia_dlugosc_sciezki << endl;
                if (najkrotsza_dlugosc == 0 || najkrotsza_dlugosc > srednia_dlugosc_sciezki)
                {
                    najkrotsza_dlugosc = srednia_dlugosc_sciezki;
                }
            }
        }while(duration.count() < czas);

        if (chociaz_jeden_sie_zrobil)
        {
            cout << "Najkrotsza sciezka dla tej populacji: " << najkrotsza_dlugosc << endl;
        }
        else
        {
            cout << "Test nie zdazyl sie wykonac w zadanym czasie. Potrzebowal " << duration.count() << " sekund." << endl;
        }
    }
}

void test_wierzcholkow(int czas)
{
    cout << "---------------------\n  TEST WIERZCHOLKOW  \n---------------------" << endl;

    std::chrono::seconds duration;
    float srednia_dlugosc_sciezki;
    const int ilosc_wierzcholkow_size = 5;
    int ilosc_wierzcholkow[] = {10,30,50,70,100};
    for (int i = 0; i < ilosc_wierzcholkow_size; i++)
    {
        generuj_graf(ilosc_wierzcholkow[i],1,6);
        auto start = std::chrono::high_resolution_clock::now();
        float prawdopodobienstwo = 0;
        bool chociaz_jeden_sie_zrobil = false;
        cout << "\nTest dla ilosci wierzcholkow: " << ilosc_wierzcholkow[i] << endl;
        float najkrotsza_dlugosc = 0;
        do
        {
            srednia_dlugosc_sciezki = wiele_instancji(prawdopodobienstwo,100);
            parowanie_feromonow(0.5);
            wygladzanie_feromonow();
            auto finish = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::seconds>(finish - start);
            if (duration.count() < czas)
            {
                chociaz_jeden_sie_zrobil = true;
                cout << "Czas trwania: " << duration.count() << " sekund, Srednia dlugosc sciezki: " << srednia_dlugosc_sciezki << endl;
                if (najkrotsza_dlugosc == 0 || najkrotsza_dlugosc > srednia_dlugosc_sciezki)
                {
                    najkrotsza_dlugosc = srednia_dlugosc_sciezki;
                }
            }
        }while(duration.count() < czas);

        if (chociaz_jeden_sie_zrobil)
        {
            cout << "Najkrotsza sciezka dla tej populacji: " << najkrotsza_dlugosc << endl;
        }
        else
        {
            cout << "Test nie zdazyl sie wykonac w zadanym czasie. Potrzebowal " << duration.count() << " sekund." << endl;
        }
    }
}

void test_stopni(int czas)
{
    cout << "--------------------\n     TEST STOPNI    \n--------------------" << endl;

    std::chrono::seconds duration;
    float srednia_dlugosc_sciezki;
    const int ilosc_par_stopni = 2;
    float stopnie_min[] = {1,4};
    float stopnie_max[] = {2,6};

    for (int i = 0; i < ilosc_par_stopni; i++)
    {
        float stopien_min = stopnie_min[i];
        float stopien_max = stopnie_max[i];
        generuj_graf(100,stopien_min, stopien_max);
        auto start = std::chrono::high_resolution_clock::now();
        float prawdopodobienstwo = 0;
        bool chociaz_jeden_sie_zrobil = false;
        cout << "\nTest dla stopni od " << stopnie_min[i] << " do " << stopnie_max[i] << endl;
        float najkrotsza_dlugosc = 0;
        do
        {
            srednia_dlugosc_sciezki = wiele_instancji(prawdopodobienstwo,100);
            parowanie_feromonow(0.5);
            wygladzanie_feromonow();
            auto finish = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::seconds>(finish - start);
            if (duration.count() < czas)
            {
                chociaz_jeden_sie_zrobil = true;
                cout << "Czas trwania: " << duration.count() << " sekund, Srednia dlugosc sciezki: " << srednia_dlugosc_sciezki << endl;
                if (najkrotsza_dlugosc == 0 || najkrotsza_dlugosc > srednia_dlugosc_sciezki)
                {
                    najkrotsza_dlugosc = srednia_dlugosc_sciezki;
                }
            }
        }while(duration.count() < czas);

        if (chociaz_jeden_sie_zrobil)
        {
            cout << "Najkrotsza sciezka dla tej populacji: " << najkrotsza_dlugosc << endl;
        }
        else
        {
            cout << "Test nie zdazyl sie wykonac w zadanym czasie. Potrzebowal " << duration.count() << " sekund." << endl;
        }
    }
}

test_parowania(int czas)
{
    cout << "--------------------\n   TEST PAROWANIA   \n--------------------" << endl;

    std::chrono::seconds duration;
    float srednia_dlugosc_sciezki;
    const int ilosc_wartosci = 4;
    float wartosci_parowania[] = {0.1, 0.5, 1, 5};

    for (int i = 0; i < ilosc_wartosci; i++)
    {
        float wartosc_parowania = wartosci_parowania[i];
        generuj_graf(100,1, 6);
        auto start = std::chrono::high_resolution_clock::now();
        float prawdopodobienstwo = 0;
        bool chociaz_jeden_sie_zrobil = false;
        cout << "\nTest parowania dla wartosci " << wartosc_parowania << endl;
        float najkrotsza_dlugosc = 0;
        do
        {
            srednia_dlugosc_sciezki = wiele_instancji(prawdopodobienstwo,100);
            parowanie_feromonow(wartosc_parowania);
            wygladzanie_feromonow();
            auto finish = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::seconds>(finish - start);
            if (duration.count() < czas)
            {
                chociaz_jeden_sie_zrobil = true;
                cout << "Czas trwania: " << duration.count() << " sekund, Srednia dlugosc sciezki: " << srednia_dlugosc_sciezki << endl;
                if (najkrotsza_dlugosc == 0 || najkrotsza_dlugosc > srednia_dlugosc_sciezki)
                {
                    najkrotsza_dlugosc = srednia_dlugosc_sciezki;
                }
            }
        }while(duration.count() < czas);

        if (chociaz_jeden_sie_zrobil)
        {
            cout << "Najkrotsza sciezka dla tej populacji: " << najkrotsza_dlugosc << endl;
        }
        else
        {
            cout << "Test nie zdazyl sie wykonac w zadanym czasie. Potrzebowal " << duration.count() << " sekund." << endl;
        }
    }
}


int main()
{
    srand (time(NULL));

    generuj_graf(100,1,6); //ilosc wierzcholkow, stopien

    test_czasu(9,30,60,300,600,900,1200,1500,1800,3600); //(ilość testów, testy w sekundach)
    wyswietl_graf();
    test_populacji(1500); //czas
    test_wierzcholkow(1500);
    test_stopni(1500);
    test_parowania(1500);
    int a;
    cin >> a;

    return 0;
}
