#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

ifstream f("date.in");

int a[501][501];
int b[501][501];
int v[501][501][501];
int N;

pthread_barrier_t barrier;

void readInput()
{
    f >> N;

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            f >> a[i][j];

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            f >> b[i][j];
}

void printAnswer()
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
            cout << v[i][j][0] << " ";

        cout << '\n';
    }
}

void *multiplication(void *args)
{
    int threadNumber = *((int *)args);
    int i = (threadNumber / N) % N;
    int j = threadNumber % N;
    int k = threadNumber / (N * N);

    v[i][j][k] = a[i][k] * b[k][j];

    int stop = log2(N);

    pthread_barrier_wait(&barrier);

    for (int m = 0; m < stop; ++m)
    {
        if (k % (1 << (m + 1)) == 0)
            v[i][j][k] += v[i][j][k + (1 << m)];
    }

    return NULL;
}

int main()
{
    readInput();

    int threadNumber = N * N * N;
    int idx[threadNumber];
    pthread_t threads[threadNumber];

    if (pthread_barrier_init(&barrier, NULL, threadNumber))
    {
        cout << "Error on barrier init\n";
        return 1;
    }

    for (int i = 0; i < threadNumber; ++i)
    {
        idx[i] = i;
        int rc = pthread_create(&threads[i], NULL, &multiplication, &idx[i]);
        if (rc != 0)
        {
            cout << "Creation failed failed\n";
            return 1;
        }
    }

    for (int i = 0; i < threadNumber; ++i)
        pthread_join(threads[i], NULL);

    pthread_barrier_destroy(&barrier);

    printAnswer();

    return 0;
}