// AM considerat numaruld e procesoare ca fiind 4
#include <bits/stdc++.h>

using namespace std;

int pNumber = 4;
map<int, string> states;
int cNumber = -1;
bool containsInformation = false;

string processorDecision(int pid, string action)
{
    if (states[pid] == "I")
        if (action == "w")
            return "X";
        else
        {
            for (const auto &[state, stateValue] : states)
                if (stateValue == "S" || stateValue == "E" || stateValue == "M")
                    return "S";

            return "#";
        }

    else if (states[pid] == "S")
    {
        if (action == "w")
            return "X";
        else
        {
            if (states[pid] == "S")
                return "-";

            for (const auto &[state, stateValue] : states)
                if (stateValue == "S" || stateValue == "E" || stateValue == "M")
                    return "F";
        }
    }

    return "-";
}

void modifyProcessor(int pid, string paction)
{
    if (states[pid] == "I")
        if (paction == "w")
        {
            states[pid] = "M";
            return;
        }
        else
        {
            for (const auto &[state, stateValue] : states)
                if (stateValue == "S" || stateValue == "E" || stateValue == "M")
                {
                    states[pid] = "S";
                    cNumber = state;
                    return;
                }

            states[pid] = "E";
            return;
        }
    else if (states[pid] == "S" && paction == "w")
        states[pid] = "M";
}

void changeProcessor(int pid, string paction)
{
    for (const auto &[state, stateValue] : states)
    {
        if (state == pid)
            continue;

        if (stateValue == "M")
            states[state] = (paction == "w") ? "I" : "S";
        else if (stateValue == "E")
            states[state] = (paction == "w") ? "I" : "S";
        else if (stateValue == "S")
            states[state] = (paction == "w") ? "I" : "S";
        else if (stateValue == "I")
            states[state] = (paction == "w") ? "I" : "I";
    }
}

string sourceParser(string character)
{
    if (character == "#")
        return "Mem";

    if (character == "S")
        return "Cache" + to_string(cNumber);

    if (character == "-")
        return "-";

    if (character == "X")
        return "Flush";

    return "";
}

string actionParser(string character)
{
    if (character == "#")
    {
        containsInformation = true;
        return "BusRd";
    }

    if (character == "X")
    {
        containsInformation = false;
        return "BusRdX";
    }

    if (character == "-")
        return "-     ";

    if (character == "S")
        if (containsInformation == 1)
            return "Flush";
        else
            return "BusRd";

    return "";
}

int main(int argc, char *argv[])
{
    unordered_map<int, string> states;
    for (int i = 0; i < pNumber; ++i)
    {
        states[i] = "I";
    }

    if (argc % 2 == 0 || argc == 1)
    {
        exit(1);
    }

    cout << "t Act_Proc Stari Act_Mag Srs_date\n";
    cout << "t0 init    ";

    for (const auto &[state, stateValue] : states)
    {
        cout << stateValue << " ";
    }
    cout << "-      -\n";

    for (int i = 1; i < argc; i += 2)
    {
        int pid = stoi(argv[i]);
        string paction = argv[i + 1];
        string action = "", source = "", statesOutput = "";
        string current_state = states[pid];

        action = processorDecision(pid, paction);
        modifyProcessor(pid, paction);
        changeProcessor(pid, paction);

        for (int proc = 0; proc < pNumber; ++proc)
        {
            statesOutput += states[proc] + " ";
        }

        cout << "t" << (i + 1) / 2 << " P" << pid << ((paction == "r") ? "Rd    " : "Wr    ") << statesOutput << " " << actionParser(action) << " " << sourceParser(action) << "\n";
    }

    return 0;
}
