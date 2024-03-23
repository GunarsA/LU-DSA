#include <iostream>
#include <fstream>

using namespace std;

#define IN_FILE "customs2.in"
#define OUT_FILE "customs2.out"

const int MAX_OFFICER_CNT = 100;
const int MAX_HEAP_SIZE = 1E3;

struct Arrival
{
    bool is_citizen;
    int arrival_time;
    int exit_time;
    int officer_index;

    bool operator<(const Arrival &other) const
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

    bool operator>(const Arrival &other) const
    {
        if (exit_time != other.exit_time)
        {
            return exit_time < other.exit_time;
        }
        else if (is_citizen != other.is_citizen)
        {
            return other.is_citizen;
        }
        else
        {
            return officer_index < other.officer_index;
        }
    }
};

struct Officer
{
    int index;
    int processing_time;
    int next_free_time;
    Arrival arrive;

    Officer() {}

    Officer(int index, int processing_time) : index(index), processing_time(processing_time), next_free_time(-1) {}

    bool operator<(const Officer &other) const
    {
        if (next_free_time != other.next_free_time)
        {
            return next_free_time < other.next_free_time;
        }
        else
        {
            return index < other.index;
        }
    }

    bool operator>(const Officer &other) const
    {
        if (next_free_time != other.next_free_time)
        {
            return next_free_time < other.next_free_time;
        }
        else
        {
            return index < other.index;
        }
    }
};

/**
 * @brief A template class representing a MinHeap data structure.
 *
 * @tparam T The type of elements stored in the MinHeap.
 */
template <typename T>
class MinHeap
{
private:
    T *arr;       // Pointer to the array storing the elements
    int capacity; // Maximum number of elements the MinHeap can hold
    int size;     // Current number of elements in the MinHeap

    /**
     * @brief Helper function to maintain the heap property by heapifying the subtree rooted at index i.
     *
     * @param i The index of the subtree to heapify.
     */
    void heapify(int i)
    {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && arr[left] < arr[smallest])
        {
            smallest = left;
        }

        if (right < size && arr[right] < arr[smallest])
        {
            smallest = right;
        }

        if (smallest != i)
        {
            swap(arr[i], arr[smallest]);
            heapify(smallest);
        }
    }

public:
    /**
     * @brief Constructs a new MinHeap object with the specified capacity.
     *
     * @param capacity The maximum number of elements the MinHeap can hold.
     */
    MinHeap(int capacity)
    {
        this->capacity = capacity;
        this->size = 0;
        this->arr = new T[capacity];
    }

    /**
     * @brief Destroys the MinHeap object and frees the memory allocated for the array.
     */
    ~MinHeap()
    {
        delete[] arr;
    }

    /**
     * @brief Inserts a new element into the MinHeap.
     *
     * @param p The element to be inserted.
     */
    void push(T p)
    {
        if (size == capacity)
        {
            return;
        }

        arr[size] = p;
        ++size;

        int i = size - 1;
        while (i != 0 && arr[i] < arr[(i - 1) / 2])
        {
            swap(arr[i], arr[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    /**
     * @brief Returns the minimum element in the MinHeap.
     *
     * @return T The minimum element.
     */
    T top()
    {
        return arr[0];
    }

    /**
     * @brief Removes the minimum element from the MinHeap.
     */
    void pop()
    {
        if (size == 0)
        {
            return;
        }

        arr[0] = arr[size - 1];
        --size;

        heapify(0);
    }

    /**
     * @brief Checks if the MinHeap is empty.
     *
     * @return true if the MinHeap is empty, false otherwise.
     */
    bool empty()
    {
        return size == 0;
    }

    /**
     * @brief Returns the current number of elements in the MinHeap.
     *
     * @return int The number of elements in the MinHeap.
     */
    int get_size()
    {
        return size;
    }
};

/**
 * @brief Process function handles the arrival of a new entry and manages the officers' availability.
 *
 * @param exit The heap storing the arrive exit times.
 * @param free The heap storing the free officers.
 * @param busy The heap storing the busy officers.
 * @param entry The new arrival entry.
 */
void process(MinHeap<Arrival> &exit, MinHeap<Officer> &free, MinHeap<Officer> &busy, Arrival entry)
{
    while (!busy.empty() && busy.top().next_free_time <= entry.arrival_time)
    {
        Officer officer = busy.top();
        busy.pop();

        exit.push(officer.arrive);

        officer.next_free_time = -1;
        free.push(officer);
    }

    if (free.empty())
    {
        Officer officer = busy.top();
        busy.pop();

        exit.push(officer.arrive);

        entry.exit_time = officer.next_free_time + officer.processing_time;
        entry.officer_index = officer.index;

        officer.next_free_time = entry.exit_time;
        officer.arrive = entry;
        busy.push(officer);
    }
    else
    {
        Officer officer = free.top();
        free.pop();

        entry.exit_time = entry.arrival_time + officer.processing_time;
        entry.officer_index = officer.index;

        officer.next_free_time = entry.exit_time;
        officer.arrive = entry;

        busy.push(officer);
    }
}

int main()
{
    // -------------------------------- Boost I/O Speed ---------------------------------
    cin.tie(0);
    ios_base::sync_with_stdio(false);

    // -------------------------------- Open input file ---------------------------------
    ifstream fin(IN_FILE, ios::in | ios::binary);

    int citizen_officer_cnt, foreigner_officer_cnt, citizen_time, foreigner_time;
    fin >> citizen_officer_cnt >> foreigner_officer_cnt >> citizen_time >> foreigner_time;

    // ----------------------------- Process default times ------------------------------
    int officers[2][MAX_OFFICER_CNT][2];
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

    // --------------------------- Process non-default times ----------------------------
    char prefix;
    fin >> prefix;
    while (prefix == 'T')
    {
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

        fin >> prefix;
    }

    // -------------------------------- Initialize Heaps --------------------------------
    MinHeap<Officer> free_citizen_officers(MAX_OFFICER_CNT), free_foreigner_officers(MAX_OFFICER_CNT);
    for (int i = 0; i < citizen_officer_cnt; ++i)
    {
        Officer officer(i, officers[0][i][0]);
        free_citizen_officers.push(officer);
    }
    MinHeap<Officer> busy_citizen_officers(MAX_OFFICER_CNT), busy_foreigner_officers(MAX_OFFICER_CNT);
    for (int i = 0; i < foreigner_officer_cnt; ++i)
    {
        Officer officer(i, officers[1][i][0]);
        free_foreigner_officers.push(officer);
    }
    MinHeap<Arrival> citizen_exit(MAX_HEAP_SIZE), foreigner_exit(MAX_HEAP_SIZE);

    // ------------------------------- Initialize Streams -------------------------------
    streampos citizen_pos = fin.tellg(), foreigner_pos = fin.tellg();
    char citizen_prefix = prefix, foreigner_prefix = prefix;

    // ------------------------------ Initialize Variables ------------------------------
    int arrival_cnt = 0;

    // ----------------------------- Redirect output stream -----------------------------
    (void)!freopen(OUT_FILE, "w", stdout);

    while (true)
    {
        // ------------------------------ Process citizens ------------------------------
        fin.seekg(citizen_pos);
        while (citizen_exit.get_size() < MAX_HEAP_SIZE && citizen_prefix != 'X')
        {
            int arrival_time;
            fin >> arrival_time;

            if (citizen_prefix == 'P')
            {
                ++arrival_cnt;

                Arrival entry = {true, arrival_time, -1, -1};
                process(citizen_exit, free_citizen_officers, busy_citizen_officers, entry);
            }

            fin >> citizen_prefix;
        }
        if (citizen_prefix == 'X')
        {
            while (!busy_citizen_officers.empty())
            {
                Arrival entry = busy_citizen_officers.top().arrive;
                citizen_exit.push(entry);
                busy_citizen_officers.pop();
            }
        }
        citizen_pos = fin.tellg();

        // ----------------------------- Process foreigners -----------------------------
        fin.seekg(foreigner_pos);
        while (foreigner_exit.get_size() < MAX_HEAP_SIZE && foreigner_prefix != 'X')
        {
            int arrival_time;
            fin >> arrival_time;

            if (foreigner_prefix == 'N')
            {
                ++arrival_cnt;

                Arrival entry = {false, arrival_time, -1, -1};
                process(foreigner_exit, free_foreigner_officers, busy_foreigner_officers, entry);
            }

            fin >> foreigner_prefix;
        }
        if (foreigner_prefix == 'X')
        {
            while (!busy_foreigner_officers.empty())
            {
                Arrival entry = busy_foreigner_officers.top().arrive;
                foreigner_exit.push(entry);
                busy_foreigner_officers.pop();
            }
        }
        foreigner_pos = fin.tellg();

        // --------------------------------- Exit process -------------------------------
        if (citizen_exit.empty() && foreigner_exit.empty())
        {
            break;
        }

        // ------------------------------ Print exit times ------------------------------
        while (!citizen_exit.empty() && !foreigner_exit.empty())
        {
            Arrival citizen = citizen_exit.top();
            Arrival foreigner = foreigner_exit.top();

            if (citizen < foreigner)
            {
                cout << citizen.arrival_time << " " << citizen.exit_time << "\n";
                citizen_exit.pop();
            }
            else
            {
                cout << foreigner.arrival_time << " " << foreigner.exit_time << "\n";
                foreigner_exit.pop();
            }
        }
        while (!citizen_exit.empty() && foreigner_prefix == 'X')
        {
            Arrival citizen = citizen_exit.top();
            cout << citizen.arrival_time << " " << citizen.exit_time << "\n";
            citizen_exit.pop();
        }
        while (!foreigner_exit.empty() && citizen_prefix == 'X')
        {
            Arrival foreigner = foreigner_exit.top();
            cout << foreigner.arrival_time << " " << foreigner.exit_time << "\n";
            foreigner_exit.pop();
        }
    }

    if (!arrival_cnt)
    {
        cout << "nothing"
             << "\n";
    }

    fin.close();

    return 0;
}