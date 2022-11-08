#include <bits/stdc++.h>
using namespace std;

ifstream f("date.in");

int a[1001][1001];
int b[1001][1001];
int c[1001][1001];
int N;

void readInput()
{
    f >> N;

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            f >> a[i][j];
        }
    }

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            f >> b[i][j];
        }
    }
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

int main()
{
    readInput();

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            c[i][j] = 0;

            for (int k = 0; k < N; ++k)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    printAnswer();

    return 0;
}