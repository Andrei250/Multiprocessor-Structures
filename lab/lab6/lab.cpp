#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

ifstream f("date.in");

vector<int> numbers;
int n;

void read()
{
    f >> n;

    for (int i = 0; i < n; ++i)
    {
        int x;
        f >> x;

        numbers.push_back(x);
    }
}

void printArray()
{
    for (int i = 0; i < n; ++i)
    {
        cout << numbers[i] << " ";
    }

    cout << '\n';
}

void sortElements()
{
    for (int i = 0; i < n - 1; i++)
    {
        int numberOfElements = n - i;

#pragma omp parallel
        {
            int numberOfThreads = omp_get_num_threads();
            int currentThread = omp_get_thread_num();
            int chunkSize = numberOfElements / numberOfThreads;
            int start = chunkSize * currentThread;
            int stop = (currentThread + 1) * chunkSize - 1;

            if (currentThread == numberOfThreads - 1)
            {
                stop = numberOfElements - 1;
            }

            for (int j = start; j < stop; ++j)
            {
                if (numbers[j] > numbers[j + 1])
                    swap(numbers[j], numbers[j + 1]);
            }

#pragma omp barrier

#pragma omp master
            {
                for (int j = 0; j < numberOfThreads - 1; ++j)
                {
                    int index = (j + 1) * chunkSize - 1;

                    if (numbers[index] > numbers[numberOfElements - 1])
                    {
                        swap(numbers[index], numbers[numberOfElements - 1]);
                    }
                }
            }
        }
    }
}

int main()
{
    read();
    sortElements();
    printArray();

    return 0;
}
