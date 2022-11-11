#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

ifstream f("input.txt");
ofstream g("serial.txt");

int a[1000001];
int n;

void CocktailSort(int a[], int n)
{
    bool swapped = true;
    int start = 0;
    int end = n - 1;

    while (swapped)
    {
        swapped = false;

        for (int i = start; i < end; ++i)
        {
            if (a[i] > a[i + 1])
            {
                swap(a[i], a[i + 1]);
                swapped = true;
            }
        }

        if (!swapped)
            break;

        swapped = false;
        --end;

        for (int i = end - 1; i >= start; --i)
        {
            if (a[i] > a[i + 1])
            {
                swap(a[i], a[i + 1]);
                swapped = true;
            }
        }

        ++start;
    }
}

void printArray(int a[], int n)
{
    for (int i = 0; i < n; i++)
        g << a[i] << " ";
    g << '\n';
}

void read()
{
    f >> n;

    for (int i = 0; i < n; ++i)
    {
        f >> a[i];
    }
}

int main()
{
    read();
    double t1 = omp_get_wtime();

    CocktailSort(a, n);

    t1 = omp_get_wtime() - t1;

    g << "Timp de executie sortare:" << t1 << '\n';

    printArray(a, n);
    return 0;
}