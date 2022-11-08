#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

ifstream f("input.txt");

int a[1000001];
int n;

void printArray(int a[], int n)
{
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << '\n';
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
            int chunkSize = (end - start + 1) / threads;
            int left = start + threadNum * chunkSize;
            int right = min(start + (threadNum + 1) * chunkSize - 1, end);

            if (threadNum == omp_get_num_threads() - 1)
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
                    int index = start + (i + 1) * chunkSize - 1;

                    if (a[index] > a[end])
                    {
                        swap(a[index], a[end]);
                        swapped = true;
                    }
                }
            }
        }

        if (!swapped)
            break;

        swapped = false;
        --end;

#pragma omp parallel
        {
            int threadNum = omp_get_thread_num();
            int threads = omp_get_num_threads();
            int chunkSize = (end - start + 1) / threads;
            int left = start + threadNum * chunkSize;
            int right = start + (threadNum + 1) * chunkSize - 1;

            if (threadNum == omp_get_num_threads() - 1)
            {
                right = end;
            }

#pragma omp barrier

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
                    int index = start + i * chunkSize;

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

    double t1 = omp_get_wtime();

    CocktailSort();

    t1 = omp_get_wtime() - t1;

    cout << "Timp de executie sortare:" << t1 << '\n';

    printArray(a, n);

    return 0;
}