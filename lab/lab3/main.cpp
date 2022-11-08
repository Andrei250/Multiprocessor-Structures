#include <bits/stdc++.h>

using namespace std;

int k, n, m;
vector<pair<int, int>> inputs;

void read()
{
    cout << "Introduceti numarul de etaje:\n";
    cin >> k;
    cout << "Introduceti numarul de conexiuni:\n";
    cin >> m;

    cout << "Introduceti perechile de intrari:\n";

    n = (1 << k);

    for (int i = 0; i < m; ++i)
    {
        int x, y;

        cin >> x >> y;
        inputs.push_back({x, y});
    }
}

void findEquivalent(int position)
{
    inputs[position].first = (inputs[position].first * 2 + (int)floor(inputs[position].first * 2 / n)) % n;
}

void findTransmissionType(int position, int transmissionType, int floor)
{
    cout << "Pe etajul " << floor << " s-a intrat pe intrarea " << inputs[position].first << " din blocul " << inputs[position].first / 2 << " si s-a iesit pe ";

    if (transmissionType == 0)
    {
        cout << inputs[position].first << " cu tipul transmisiunii DIRECTA cu C0 = C1 = 0\n";
    }
    else
    {
        inputs[position].first += inputs[position].first % 2 == 0 ? 1 : -1;

        cout << inputs[position].first << " cu tipul transmisiunii INDIRECTA cu C0 = C1 = 1\n";
    }
}

void solve()
{
    for (int i = 0; i < m; ++i)
    {
        int transmissions = inputs[i].first ^ inputs[i].second;

        cout << "Start:\n";

        cout << inputs[i].first << '\n';

        // mutam primele intrari in primul rand de blocuri
        findEquivalent(i);

        for (int j = 0; j < k; ++j)
        {

            int bitPosition = (k - j - 1);
            int transmissionType = (transmissions & (1 << bitPosition)) >> bitPosition;
            findTransmissionType(i, transmissionType, j);
            findEquivalent(i);
        }
    }
}

int main()
{
    read();
    solve();

    return 0;
}