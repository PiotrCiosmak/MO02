#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <iomanip>

using namespace std;

void wczytaj(vector<long double> *log10x, vector<long double> *x, vector<long double> *fx);

long double taylor(long double x, int n);

void zapisz(const vector<long double> *log10x, const vector<long double> *fx_log10, const vector<long double> *fx_log10_taylor, size_t ilosc);

int main()
{
    vector<long double> log10x_dane;
    vector<long double> x_dane;
    vector<long double> fx_dane;
    wczytaj(&log10x_dane, &x_dane, &fx_dane);
    size_t ilosc_wierszy{x_dane.size()};

    vector<long double> fx_obliczone;
    for (auto element: fx_dane)
    {
        fx_obliczone.push_back((1.0 - exp(-element)) / element);
    }

    vector<long double> blad_wzgledny_obliczone;
    for (size_t i = 0; i < ilosc_wierszy; ++i)
    {
        blad_wzgledny_obliczone.push_back(fabs((fx_dane[i] - fx_obliczone[i]) / fx_dane[i]));
    }

    vector<long double> fx_log10_obliczone;
    for (auto element: blad_wzgledny_obliczone)
    {
        fx_log10_obliczone.push_back(log10(element));
    }

    //2 czesc
    vector<long double> fx_taylor_obliczone;
    for (auto element: fx_dane)
    {
        fx_taylor_obliczone.push_back(taylor(element, 9999));
    }

    vector<long double> blad_wzgledny_taylor_obliczone;
    for (size_t i = 0; i < ilosc_wierszy; ++i)
    {
        blad_wzgledny_taylor_obliczone.push_back(fabs((fx_dane[i] - fx_taylor_obliczone[i]) / fx_dane[i]));
    }

    vector<long double> fx_log10_taylor_obliczone;
    for (auto element: blad_wzgledny_taylor_obliczone)
    {
        fx_log10_taylor_obliczone.push_back(log10(element));
    }
    zapisz(&log10x_dane, &fx_log10_obliczone, &fx_log10_taylor_obliczone, ilosc_wierszy);

}

void wczytaj(vector<long double> *log10x, vector<long double> *x, vector<long double> *fx)
{
    ifstream plik("../dane.txt");
    string linia;
    if (plik.is_open())
    {
        while (getline(plik, linia))
        {
            size_t first_tab{linia.find('\t')};
            size_t second_tab{linia.find('\t', first_tab + 1)};
            (*log10x).push_back(stold(linia.substr(0, first_tab)));
            (*x).push_back(stold(linia.substr(first_tab + 1, second_tab)));
            (*fx).push_back(stold(linia.substr(second_tab + 1)));
        }
        plik.close();
    }
}

long double taylor(long double x, int n)
{
    long double suma{};
    long double tempX{1};
    long double tempSilnia{1};
    for (int i = 1; i <= n; ++i)
    {
        tempSilnia *= i;
        if (i % 2 == 0)
        {
            suma += tempX / tempSilnia;
        }
        else
        {
            suma -= tempX / tempSilnia;
        }
        tempX *= x;
    }
    return suma;
}

void zapisz(const vector<long double> *log10x, const vector<long double> *fx_log10, const vector<long double> *fx_log10_taylor, size_t ilosc)
{
    ofstream plik1("../dane1.txt");
    ofstream plik2("../dane2.txt");

    if (plik1.is_open() && plik2.is_open())
    {
        for (size_t i = 0; i < ilosc; ++i)
        {
            plik1 << setprecision(100) << log10x->at(i) << " " << setprecision(100) << fx_log10->at(i) << "\n";
            plik2 << setprecision(100) << log10x->at(i) << " " << setprecision(100) << fx_log10_taylor->at(i) << "\n";
        }
        plik1.close();
        plik2.close();
    }
}