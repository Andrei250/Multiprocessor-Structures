#include <bits/stdc++.h>
#include "mpi.h"

#define MASTER 0

using namespace std;

ifstream f("input.txt");

int a[1000001];
int n;
int start, stop;
int processes, process_id;
int swapped = 1;
MPI_Status status;

void printArray(int a[], int n)
{
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << '\n';
}

void read()
{
    f >> n;

    for (int i = 0; i < n; ++i)
    {
        f >> a[i];
    }
}

void masterNode()
{
    read();

    start = 0;
    stop = n - 1;

    for (int i = 1; i < processes; ++i)
    {
        MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    while (swapped)
    {
        swapped = 0;

        for (int i = 1; i < processes; ++i)
        {
            int chunkSize = (stop - start - 1) / (processes - 1);
            int inceput, sfarsit;

            inceput = start + (i - 1) * chunkSize;
            sfarsit = start + i * chunkSize - 1;

            if (i == processes - 1)
            {
                sfarsit = stop;
            }

            MPI_Send(&inceput, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&sfarsit, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&a[inceput], sfarsit - inceput + 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        for (int i = 1; i < processes; ++i)
        {
            int chunkSize = (stop - start - 1) / (processes - 1);
            int inceput, sfarsit, answer;

            inceput = start + (i - 1) * chunkSize;
            sfarsit = start + i * chunkSize - 1;

            if (i == processes - 1)
            {
                sfarsit = stop;
            }

            MPI_Recv(&answer, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&a[inceput], sfarsit - inceput + 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

            swapped = swapped || answer;
        }

        for (int i = 1; i < processes; ++i)
        {
            int chunkSize = (stop - start - 1) / (processes - 1);
            int inceput, sfarsit, answer;

            inceput = start + (i - 1) * chunkSize;
            sfarsit = start + i * chunkSize - 1;

            if (i == processes - 1)
            {
                sfarsit = stop;
            }

            if (a[sfarsit] > a[stop])
            {
                swap(a[sfarsit], a[stop]);
                swapped = 1;
            }

            if (a[start] > a[inceput])
            {
                swap(a[inceput], a[start]);
                swapped = 1;
            }
        }

        start++;
        stop--;

        for (int i = 1; i < processes; ++i)
        {
            MPI_Send(&swapped, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }

    printArray(a, n);
}

void workerNode()
{
    int left, right;
    MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

    while (swapped)
    {
        swapped = 0;

        MPI_Recv(&left, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&right, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, (right - left + 1), MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        for (int i = 0; i < right - left; ++i)
        {
            if (a[i] > a[i + 1])
            {
                swap(a[i], a[i + 1]);
                swapped = 1;
            }
        }

        for (int i = right - left - 1; i >= 0; --i)
        {
            if (a[i] > a[i + 1])
            {
                swap(a[i], a[i + 1]);
                swapped = 1;
            }
        }

        MPI_Send(&swapped, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&a, right - left + 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&swapped, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    if (process_id == MASTER)
    {
        masterNode();
    }
    else
    {
        workerNode();
    }

    MPI_Finalize();
}