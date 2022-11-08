#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <string>
#include <algorithm>

using namespace std;

struct blockStructure
{
    int connectionType;
    int direction[2];
};

int n, nivele;
blockStructure bktrMatrix[1001][1001];

void initializeazaReteaua()
{
    int i = 0, j = 0, direction;

    while (i < n / 2)
    {
        bktrMatrix[0][i].direction[0] = i;
        bktrMatrix[0][i].direction[1] = n / 2 + i;

        i++;
    }

    i = 0;

    while (i < n / 2)
    {
        bktrMatrix[3][i / 2].direction[i % 2] = 2 * i;
        i++;
    }

    while (i < n)
    {
        bktrMatrix[3][i / 2].direction[i % 2] = (2 * i + 1) - n;
        i++;
    }

    i = 1;

    while (i < nivele - 2)
    {
        j = 0;

        while (j < n / 4)
        {
            bktrMatrix[i][j].direction[0] = j % (n / 2);
            bktrMatrix[i][j].direction[1] = (j + 2) % (n / 2);
            j++;
        }

        j = 0;

        while (j < n / 4)
        {
            bktrMatrix[i][j + n / 4].direction[0] = n / 2 + j % (n / 2);
            bktrMatrix[i][j + n / 4].direction[1] = n / 2 + (j + 2) % (n / 2);
            j++;
        }

        i++;
    }

    i = 0;

    while (i < n / 2)
    {
        bktrMatrix[nivele - 1][i].direction[0] = 2 * i;
        bktrMatrix[nivele - 1][i].direction[1] = 2 * i + 1;
        i++;
    }

    for (int k = 0; k < nivele; k++)
        for (int w = 0; w < n / 2; w++)
            bktrMatrix[k][w].connectionType = -1;
}

bool checkValidity(int sursa, string path)
{
    int j = 0;

    while (j < nivele)
    {
        int direction = (path[j] - '0') ^ (sursa % 2);

        if (bktrMatrix[j][sursa / 2].connectionType == -1)
            bktrMatrix[j][sursa / 2].connectionType = path[j] - '0';
        else if (bktrMatrix[j][sursa / 2].connectionType != (path[j] - '0'))
            return false;
        else
            sursa = bktrMatrix[j][sursa / 2].direction[direction];

        j++;
    }

    return true;
}

string binaryValue(int value)
{
    string answer = "";

    while (value)
    {
        answer += ((value & 1) + '0');
        value >>= 1;
    }

    reverse(answer.begin(), answer.end());

    return answer;
}

int main()
{
    int i, start, stop, m, etaj;

    cout << "Introdu N si numarul de perechi:";
    cin >> n >> m;

    nivele = 2 * floor(log2(n)) - 1;

    initializeazaReteaua();

    while (m)
    {
        cin >> start >> stop;

        int backtrack = 0;

        for (int i = 0; i < pow(2, nivele) - 1; i++)
        {
            string bktrToBinary = binaryValue(backtrack);
            int end = start;

            for (int k = 0; k < nivele; k++)
            {
                int direction = (bktrToBinary[k] - '0') ^ (end % 2);
                end = bktrMatrix[k][end / 2].direction[direction];
            }

            if (end == stop)
                if (checkValidity(start, bktrToBinary))
                {
                    etaj = 0;

                    for (int j = 0; j < bktrToBinary.size(); j++)
                        if (bktrToBinary[j] == '0')
                            cout << "Conexiunea la etajul " << etaj++ << " pe comutator este directa\n";
                        else
                            cout << "Conexiunea la etajul " << etaj++ << " pe comutator este inversa\n";

                    break;
                }

            backtrack++;
        }

        --m;
    }

    return 0;
}