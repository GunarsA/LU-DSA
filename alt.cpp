#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

const int MAX_OFFICER_CNT = 100;

struct Person
{
    bool is_citizen;
    int arrival_time;
    int exit_time;
    int officer_index;

    bool operator<(const Person &other) const
    {
        if (exit_time != other.exit_time)
        {
            return exit_time > other.exit_time;
        }
        else if (is_citizen != other.is_citizen)
        {
            return other.is_citizen;
        }
        else
        {
            return officer_index > other.officer_index;
        }
    }

    bool operator>(const Person &other) const
    {
        if (exit_time != other.exit_time)
        {
            return exit_time < other.exit_time;
        }
        else if (is_citizen != other.is_citizen)
        {
            return is_citizen;
        }
        else
        {
            return officer_index < other.officer_index;
        }
    }
};

void process(priority_queue<Person> &pq, int officers[MAX_OFFICER_CNT][3], int officer_count, int arrival_time, bool is_citizen)
{
    bool is_found = false;
    int min_index = 0;
    for (int i = 0; i < officer_count; ++i)
    {
        if (officers[i][1] <= arrival_time && officers[i][1] != -1)
        {
            pq.push({is_citizen, officers[i][2], officers[i][1], i});
            officers[i][1] = -1;
        }

        if (!is_found && officers[i][1] == -1)
        {
            officers[i][1] = arrival_time + officers[i][0];
            officers[i][2] = arrival_time;
            is_found = true;
        }

        if (officers[i][1] < officers[min_index][1])
        {
            min_index = i;
        }
    }

    if (!is_found)
    {
        pq.push({is_citizen, officers[min_index][2], officers[min_index][1], min_index});
        officers[min_index][1] += officers[min_index][0];
        officers[min_index][2] = arrival_time;
    }
}

int main()
{
    ifstream fin("customs2.in", ios::in | ios::binary);

    int officers[2][MAX_OFFICER_CNT][3];

    int citizen_officer_cnt, foreigner_officer_cnt, citizen_time, foreigner_time;
    fin >> citizen_officer_cnt >> foreigner_officer_cnt >> citizen_time >> foreigner_time;

    for (int i = 0; i < citizen_officer_cnt; ++i)
    {
        officers[0][i][0] = citizen_time;
        officers[0][i][1] = -1;
    }

    for (int i = 0; i < foreigner_officer_cnt; ++i)
    {
        officers[1][i][0] = foreigner_time;
        officers[1][i][1] = -1;
    }

    char type;
    fin >> type;

    while (true)
    {
        if (type != 'T')
        {
            break;
        }

        char category;
        int index, time;
        fin >> category >> index >> time;

        if (category == 'P')
        {
            officers[0][index - 1][0] = time;
        }
        else
        {
            officers[1][index - 1][0] = time;
        }

        fin >> type;
    }

    int arrival_cnt = 0;

    auto cmp = [](Person a, Person b) { return a > b; };
    priority_queue<Person> pq1, pq2;

    ofstream fout("customs.out", ios::out);

    streampos pos1 = fin.tellg();
    streampos pos2 = fin.tellg();
    char type1 = type, type2 = type;

    while (true)
    {
        while (pq1.empty() && type1 != 'X')
        {
            fin.seekg(pos1);

            int arrival_time;
            fin >> arrival_time;

            if (type1 == 'P')
            {
                ++arrival_cnt;
                process(pq1, officers[0], citizen_officer_cnt, arrival_time, true);
            }

            fin >> type1;

            pos1 = fin.tellg();

            if (type1 == 'X')
            {
                for(int i = 0; i < citizen_officer_cnt; ++i)
                {
                    if (officers[0][i][1] != -1)
                    {
                        pq1.push({true, officers[0][i][2], officers[0][i][1], i});
                        officers[0][i][1] = -1;
                    }
                }
            }
        }

        while (pq2.empty() && type2 != 'X')
        {
            fin.seekg(pos2);

            int arrival_time;
            fin >> arrival_time;

            if (type2 == 'N')
            {
                ++arrival_cnt;
                process(pq2, officers[1], foreigner_officer_cnt, arrival_time, false);
            }

            fin >> type2;

            pos2 = fin.tellg();

            if (type2 == 'X')
            {
                for(int i = 0; i < foreigner_officer_cnt; ++i)
                {
                    if (officers[1][i][1] != -1)
                    {
                        pq2.push({false, officers[1][i][2], officers[1][i][1], i});
                        officers[1][i][1] = -1;
                    }
                }
            }
        }

        if (pq1.empty() && pq2.empty())
        {
            break;
        }

        while (!pq1.empty() && !pq2.empty())
        {
            Person p1 = pq1.top();
            Person p2 = pq2.top();

            if (p1 > p2)
            {
                fout << p1.arrival_time << " " << p1.exit_time << endl;
                pq1.pop();
            }
            else
            {
                fout << p2.arrival_time << " " << p2.exit_time << endl;
                pq2.pop();
            }
        }

        while (!pq1.empty() && type2 == 'X')
        {
            Person p = pq1.top();
            fout << p.arrival_time << " " << p.exit_time << endl;
            pq1.pop();
        }

        while (!pq2.empty() && type1 == 'X')
        {
            Person p = pq2.top();
            fout << p.arrival_time << " " << p.exit_time << endl;
            pq2.pop();
        }
    }

    if(!arrival_cnt)
    {
        fout << "nothing" << endl;
    }

    fin.close();
    fout.close();

    return 0;
}