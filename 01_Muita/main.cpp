#include <iostream>

using namespace std;

const int MAX_OFFICER_CNT = 100;
const int MAX_ARRIVAL_CNT = 4E4;

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

    bool operator>(const Person &other) const
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
};

template <typename T>
int _partition(T arr[], int low, int high)
{
    T pivot = arr[low];
    int i = low - 1, j = high + 1;

    while (true)
    {
        do
        {
            ++i;
        } while (arr[i] < pivot);

        do
        {
            --j;
        } while (arr[j] > pivot);

        if (i >= j)
        {
            return j;
        }

        swap(arr[i], arr[j]);
    }
}

/**
 * Sorts an array using the QuickSort algorithm.
 *
 * @tparam T The type of elements in the array.
 * @param arr The array to be sorted.
 * @param low The starting index of the array.
 * @param high The ending index of the array.
 */
template <typename T>
void quickSort(T arr[], int low, int high)
{
    if (low < high)
    {
        int pivot = _partition(arr, low, high);

        quickSort(arr, low, pivot);
        quickSort(arr, pivot + 1, high);
    }
}

int main()
{
    (void)!freopen("customs.in", "r", stdin);
    (void)!freopen("customs.out", "w", stdout);

    int officers[2][MAX_OFFICER_CNT][2];
    Person persons[MAX_ARRIVAL_CNT];

    int citizen_officer_cnt, foreigner_officer_cnt, citizen_time, foreigner_time;
    cin >> citizen_officer_cnt >> foreigner_officer_cnt >> citizen_time >> foreigner_time;

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
    cin >> type;

    while (true)
    {
        if (type != 'T')
        {
            break;
        }

        char category;
        int index, time;
        cin >> category >> index >> time;

        if (category == 'P')
        {
            officers[0][index - 1][0] = time;
        }
        else
        {
            officers[1][index - 1][0] = time;
        }

        cin >> type;
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

        persons[arrival_cnt].is_citizen = (type == 'P');

        int min_index = 0, selector = (type == 'P' ? 0 : 1);
        bool is_free = false;
        for (int i = 0;
             (type == 'P' && i < citizen_officer_cnt) || (type == 'N' && i < foreigner_officer_cnt);
             ++i)
        {

            if (officers[selector][i][1] <= arrival_time)
            {
                persons[arrival_cnt].arrival_time = arrival_time;
                persons[arrival_cnt].exit_time = arrival_time + officers[selector][i][0];
                persons[arrival_cnt].officer_index = i;

                officers[selector][i][1] = arrival_time + officers[selector][i][0];

                is_free = true;

                break;
            }

            if (officers[selector][i][1] < officers[selector][min_index][1])
            {
                min_index = i;
            }
        }

        if (!is_free)
        {
            persons[arrival_cnt].arrival_time = arrival_time;
            persons[arrival_cnt].exit_time = officers[selector][min_index][1] + officers[selector][min_index][0];
            persons[arrival_cnt].officer_index = min_index;

            officers[selector][min_index][1] += officers[selector][min_index][0];
        }

        cin >> type;

        ++arrival_cnt;
    }

    if (!arrival_cnt)
    {
        cout << "nothing" << endl;
    }
    else
    {
        quickSort(persons, 0, arrival_cnt - 1);

        for (int i = 0; i < arrival_cnt; ++i)
        {
            cout << persons[i].arrival_time << " " << persons[i].exit_time << endl;
        }
    }

    return 0;
}