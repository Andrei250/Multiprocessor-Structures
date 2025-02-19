#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

ifstream f("input.txt");
ofstream g("openmp.txt");

int a[1000001];
int n;

// metoda care afiseaza array-ul
void printArray(int a[], int n)
{
    for (int i = 0; i < n; i++)
        g << a[i] << " ";
    g << '\n';
}

void CocktailSort()
{
    bool swapped = true;
    int start = 0;
    int end = n - 1;

    while (swapped)
    {
        swapped = false;

#pragma omp parallel
        {
            int threadNum = omp_get_thread_num();
            int threads = omp_get_num_threads();
            int chunkSize = max(1, (end - start + 1) / threads);
            int left = min(start + threadNum * chunkSize, end);
            int right = min(start + (threadNum + 1) * chunkSize - 1, end);

            if (threadNum == threads - 1)
            {
                right = end;
            }

            for (int i = left; i < right; ++i)
            {
                if (a[i] > a[i + 1])
                {
                    swap(a[i], a[i + 1]);
                    swapped = true;
                }
            }

#pragma omp barrier

#pragma omp master
            {
                for (int i = 0; i < threads - 1; ++i)
                {
                    int index = min(start + (i + 1) * chunkSize - 1, end);

                    if (a[index] > a[end])
                    {
                        swap(a[index], a[end]);
                        swapped = true;
                    }
                }
            }
        }

        swapped = false;
        --end;

#pragma omp parallel
        {
            int threadNum = omp_get_thread_num();
            int threads = omp_get_num_threads();
            int chunkSize = max(1, (end - start + 1) / threads);
            int left = min(start + threadNum * chunkSize, end);
            int right = min(start + (threadNum + 1) * chunkSize - 1, end);

            if (threadNum == threads - 1)
            {
                right = end;
            }

            for (int i = right - 1; i >= left; --i)
            {
                if (a[i] > a[i + 1])
                {
                    swap(a[i], a[i + 1]);
                    swapped = true;
                }
            }
#pragma omp barrier

#pragma omp master
            {

                for (int i = 1; i < threads; ++i)
                {
                    int index = min(start + i * chunkSize, end);

                    if (a[index] < a[start])
                    {
                        swap(a[index], a[start]);
                        swapped = true;
                    }
                }
            }
        }

        ++start;
    }
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

    CocktailSort();

    // printArray(a, n);

    return 0;
}