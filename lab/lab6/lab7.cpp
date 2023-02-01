#include <bits/stdc++.h>
#include <omp.h>
#define NUM_THREADS 4

using namespace std;

ifstream f("date.in");

vector<int> a;
int n;
pthread_barrier_t barrier;
pthread_mutex_t mtx;
volatile bool swapped = true;
int start = 0;
int stop;

void read()
{
    f >> n;

    for (int i = 0; i < n; ++i)
    {
        int x;
        f >> x;

        a.push_back(x);
    }

    stop = n - 1;
}

void printArray()
{
    for (int i = 0; i < n; ++i)
    {
        cout << a[i] << " ";
    }

    cout << '\n';
}

void *sortElements(void *arg)
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

        chunkSize = max(1, (stop + 1) / NUM_THREADS);
        left = min(id * chunkSize, stop);
        right = min((id + 1) * chunkSize - 1, stop);

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
            for (int i = 0; i < NUM_THREADS - 1; ++i)
            {
                int index = min((i + 1) * chunkSize - 1, stop);

                if (a[index] > a[stop])
                {
                    swap(a[index], a[stop]);
                    swapped = true;
                }
            }

            stop--;
        }

        pthread_barrier_wait(&barrier);
    }

    if (id == 0)
    {
        for (int i = 0; i < NUM_THREADS; ++i)
        {
            for (int j = 0; j < NUM_THREADS; ++j)
            {
                if (a[j] > a[j + 1])
                {
                    swap(a[j], a[j + 1]);
                }
            }
        }
    }

    return NULL;
}

int main()
{
    read();

    int id, r;
    pthread_t threads[NUM_THREADS];
    int arguments[NUM_THREADS];
    void *status;

    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
    pthread_mutex_init(&mtx, NULL);

    read();

    start = 0, stop = n - 1;

    for (id = 0; id < NUM_THREADS; id++)
    {
        arguments[id] = id;
        r = pthread_create(&threads[id], NULL, sortElements, (void *)&arguments[id]);

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

    printArray();

    return 0;
}
