#include <iostream>

using namespace std;

int main()
{
    cin.tie(0);
    ios_base::sync_with_stdio(false);
    
    (void)!freopen("customs2.in", "r", stdin);
    (void)!freopen("customs2.out", "w", stdout);

    int citizen_officer_cnt, foreigner_officer_cnt, citizen_time, foreigner_time;
    cin >> citizen_officer_cnt >> foreigner_officer_cnt >> citizen_time >> foreigner_time;
    cout << citizen_officer_cnt << " " << foreigner_officer_cnt << " " << citizen_time << " " << foreigner_time << endl;

    char type;
    cin >> type;
    cout << type;

    while (true)
    {
        if (type != 'T')
        {
            break;
        }

        char category;
        int index, time;
        cin >> category >> index >> time;
        cout << " " << category << " " << index << " " << time << endl;

        cin >> type;
        cout << type;
    }

    int arrival_cnt = 0;

    while (true)
    {
        if (type == 'X')
        {
            break;
        }

        int arrival_time;
        cin >> arrival_time;
        cout << " " << arrival_time << "\n";

        cin >> type;
        cout << type;
    }

    return 0;
}