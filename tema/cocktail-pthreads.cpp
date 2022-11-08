#include <bits/stdc++.h>
#include <pthread.h>

#define NUM_THREADS 4

using namespace std;

ifstream f("input.txt");

int a[1000001];
int n;
int start, stop;
volatile bool swapped = true;

pthread_barrier_t barrier;
pthread_mutex_t mtx;

void printArray(int a[], int n)
{
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << '\n';
}

void *CocktailSort(void *arg)
{
    int id = *(int *)arg;
    int chunkSize, left, right;

    while (swapped)
    {
        // daca nu ar fi bariera aici, primul thread ar intra in if si
        // ar bloca totul
        pthread_barrier_wait(&barrier);

        if (id == 0)
        {
            swapped = false;
        }

        pthread_barrier_wait(&barrier);

        chunkSize = (stop - start + 1) / NUM_THREADS;
        left = start + id * chunkSize;
        right = start + (id + 1) * chunkSize - 1;

        if (id == NUM_THREADS - 1)
        {
            right = stop;
        }

        for (int i = left; i < right; ++i)
        {
            if (a[i] > a[i + 1])
            {
                swap(a[i], a[i + 1]);
                swapped = true;
            }
        }

        pthread_barrier_wait(&barrier);

        if (id == 0)
        {
            for (int i = 0; i < NUM_THREADS; ++i)
            {
                int index = start + (i + 1) * chunkSize - 1;

                if (a[index] > a[stop])
                {
                    swap(a[index], a[stop]);
                    swapped = true;
                }
            }
        }

        pthread_barrier_wait(&barrier);

        if (!swapped)
            break;

        pthread_barrier_wait(&barrier);

        if (id == 0)
        {
            swapped = false;
            --stop;
        }

        pthread_barrier_wait(&barrier);

        chunkSize = (stop - start + 1) / NUM_THREADS;
        left = start + id * chunkSize;
        right = start + (id + 1) * chunkSize - 1;

        if (id == NUM_THREADS - 1)
        {
            right = stop;
        }

        for (int i = right - 1; i >= start; --i)
        {
            if (a[i] > a[i + 1])
            {
                swap(a[i], a[i + 1]);
                swapped = true;
            }
        }

        pthread_barrier_wait(&barrier);

        if (id == 0)
        {

            for (int i = 1; i <= NUM_THREADS; ++i)
            {
                int index = start + i * chunkSize;

                if (a[index] < a[start])
                {
                    swap(a[index], a[start]);
                    swapped = true;
                }
            }

            ++start;
        }

        pthread_barrier_wait(&barrier);
    }

    return NULL;
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
    clock_t t1, t2;
    int r, id;
    pthread_t threads[NUM_THREADS];
    int arguments[NUM_THREADS];
    void *status;

    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
    pthread_mutex_init(&mtx, NULL);

    read();

    start = 0, stop = n - 1;

    t1 = clock();

    for (id = 0; id < NUM_THREADS; id++)
    {
        arguments[id] = id;
        r = pthread_create(&threads[id], NULL, CocktailSort, (void *)&arguments[id]);

        if (r)
        {
            cout << "Eroare la crearea thread-ului " << id << "\n";
            exit(-1);
        }
    }

    for (id = 0; id < NUM_THREADS; id++)
    {
        r = pthread_join(threads[id], &status);

        if (r)
        {
            cout << "Eroare la inchiderea thread-ului " << id << "\n";
            exit(-1);
        }
    }

    t2 = clock();

    cout.precision(10);
    cout << "Timpul de executie pthreads este: " << fixed << double(t2 - t1) / CLOCKS_PER_SEC << '\n';

    printArray(a, n);
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mtx);

    return 0;
}