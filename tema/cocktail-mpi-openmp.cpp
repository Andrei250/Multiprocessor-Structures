#include <bits/stdc++.h>
#include "mpi.h"
#include <omp.h>

#define MASTER 0

using namespace std;

ifstream f("input.txt");
ofstream g("hybrid.txt");

int a[1000001];
int n;
int start, stop;
int processes, process_id;
int swapped = 1;
MPI_Status status;

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

void masterNode()
{
    read();

    start = 0;
    stop = n - 1;

    while (swapped)
    {
        swapped = 0;

        for (int i = 1; i < processes; ++i)
        {
            int chunkSize = (stop - start + 1) / (processes - 1);
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
            int chunkSize = (stop - start + 1) / (processes - 1);
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
            int chunkSize = (stop - start + 1) / (processes - 1);
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

    while (swapped)
    {
        swapped = 0;

        MPI_Recv(&left, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&right, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, (right - left + 1), MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

#pragma omp parallel
        {
            int threadNum = omp_get_thread_num();
            int threads = omp_get_num_threads();
            int chunkSize = max(1, (right - left + 1) / threads);
            int leftV = min(threadNum * chunkSize, right - left);
            int rightV = min((threadNum + 1) * chunkSize - 1, right - left);

            if (threadNum == threads - 1)
            {
                rightV = right - left;
            }

            for (int i = leftV; i < rightV; ++i)
            {
                if (a[i] > a[i + 1])
                {
                    swap(a[i], a[i + 1]);
                    swapped = 1;
                }
            }

#pragma omp barrier

#pragma omp master
            {
                for (int i = 0; i < threads - 1; ++i)
                {
                    int index = min((i + 1) * chunkSize - 1, right - left);

                    if (a[index] > a[right - left])
                    {

                        swap(a[index], a[right - left]);
                        swapped = 1;
                    }
                }
            }

#pragma omp barrier

            for (int i = rightV - 1; i >= leftV; --i)
            {
                if (a[i] > a[i + 1])
                {
                    swap(a[i], a[i + 1]);
                    swapped = 1;
                }
            }
#pragma omp barrier

#pragma omp master
            {

                for (int i = 1; i < threads; ++i)
                {
                    int index = min(i * chunkSize, right - left);

                    if (a[index] < a[0])
                    {
                        swap(a[index], a[0]);
                        swapped = 1;
                    }
                }
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