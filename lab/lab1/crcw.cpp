#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

ifstream f("date.in");

int a[1001][1001];
int b[1001][1001];
int c[1001][1001];
int N;

pthread_mutex_t mtx[1001][1001];

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
        {
            cout << c[i][j] << " ";
        }

        cout << '\n';
    }
}

void *multiplication(void *args)
{
    int threadNumber = *((int *)args);
    int i = (threadNumber / N) % N;
    int j = threadNumber % N;
    int k = threadNumber / (N * N);

    pthread_mutex_lock(&mtx[i][j]);
    c[i][j] += a[i][k] * b[k][j];
    pthread_mutex_unlock(&mtx[i][j]);

    return NULL;
}

int main()
{
    readInput();

    int threadNumber = N * N * N;
    int idx[threadNumber];
    int rc;
    pthread_t threads[threadNumber];

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (pthread_mutex_init(&mtx[i][j], NULL) != 0)
            {
                cout << "Failed on creating mutex\n";
                return 1;
            }

    for (int i = 0; i < threadNumber; ++i)
    {
        idx[i] = i;
        rc = pthread_create(&threads[i], NULL, &multiplication, &idx[i]);
        if (rc != 0)
        {
            cout << "Creation failed\n";
            return 1;
        }
    }

    for (int i = 0; i < threadNumber; ++i)
        pthread_join(threads[i], NULL);

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            pthread_mutex_destroy(&mtx[i][j]);

    printAnswer();

    return 0;
}