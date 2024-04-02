#include <iostream>

#include <chrono>

using namespace std;

#define MAX_NAME_LEN 100
#define MAX_PSEUDONYM_CNT 50
#define MAX_REVIEWER_CNT 10000

#define INPUT_FILE "reviewers.in"
#define OUTPUT_FILE "reviewers.out"

// -------------------------------- Structures --------------------------------

struct String
/**
 * @brief Represents a string of characters.
 */
{
    char data[MAX_NAME_LEN];

    /**
     * @brief Overloaded equality operator.
     *
     * @param other The string to compare with.
     * @return true if the strings are equal, false otherwise.
     */
    bool operator==(const String &other) const
    {
        for (int i = 0; i < MAX_NAME_LEN; ++i)
        {
            if (data[i] != other.data[i])
            {
                return false;
            }

            if (data[i] == '\0')
            {
                break;
            }
        }

        return true;
    }

    /**
     * @brief Overloaded inequality operator.
     *
     * @param other The string to compare with.
     * @return true if the strings are not equal, false otherwise.
     */
    bool operator!=(const String &other) const
    {
        return !(*this == other);
    }

    /**
     * @brief Overloaded less than operator.
     *
     * @param other The string to compare with.
     * @return true if this string is less than the other string, false otherwise.
     */
    bool operator<(const String &other) const
    {
        for (int i = 0; i < MAX_NAME_LEN; ++i)
        {
            if (data[i] < other.data[i])
            {
                return true;
            }
            else if (data[i] > other.data[i])
            {
                return false;
            }

            if (data[i] == '\0')
            {
                break;
            }
        }

        return false;
    }

    /**
     * @brief Overloaded greater than operator.
     *
     * @param other The string to compare with.
     * @return true if this string is greater than the other string, false otherwise.
     */
    bool operator>(const String &other) const
    {
        for (int i = 0; i < MAX_NAME_LEN; ++i)
        {
            if (data[i] > other.data[i])
            {
                return true;
            }
            else if (data[i] < other.data[i])
            {
                return false;
            }

            if (data[i] == '\0')
            {
                break;
            }
        }
    }

    /**
     * @brief Overloaded less than or equal to operator.
     *
     * @param other The string to compare with.
     * @return true if this string is less than or equal to the other string, false otherwise.
     */
    bool operator<=(const String &other) const
    {
        return *this < other || *this == other;
    }

    /**
     * @brief Overloaded greater than or equal to operator.
     *
     * @param other The string to compare with.
     * @return true if this string is greater than or equal to the other string, false otherwise.
     */
    bool operator>=(const String &other) const
    {
        return *this > other || *this == other;
    }
};

/**
 * @brief Represents a reviewer.
 */
struct Reviewer
{
    /**
     * @brief A template class representing an array.
     *
     * This class provides a way to store and access elements in an array-like structure.
     * The size of the array is fixed and defined by the constant MAX_PSEUDONYM_CNT.
     *
     * @tparam T The type of elements stored in the array.
     */
    template <typename T>
    class Array
    {
    private:
        T data[MAX_PSEUDONYM_CNT];
        int size = MAX_PSEUDONYM_CNT;

    public:
        /**
         * @brief Accesses the element at the specified index.
         *
         * @param index The index of the element to access.
         * @return T& A reference to the element at the specified index.
         * @throws const char* If the index is out of bounds.
         */
        T &operator[](int index)
        {
            if (index >= 0 && index < size)
                return data[index];
            else
                throw "Index out of bounds";
        }
    };

    String name;
    int pseudonym_count;
    Array<int> pseudonyms;

    /**
     * @brief Default constructor for the Reviewer class.
     */
    Reviewer() {}

    /**
     * @brief Constructor for the Reviewer class.
     * @param name The name of the reviewer.
     */
    Reviewer(String name)
    {
        this->name = name;
        pseudonym_count = 0;
    }

    /**
     * @brief Overloaded equality operator for the Reviewer class.
     * @param other The other Reviewer object to compare with.
     * @return True if the reviewers have the same name, false otherwise.
     */
    bool operator==(const Reviewer &other) const
    {
        return name == other.name;
    }
};

// -------------------------------- Hash Table --------------------------------

/**
 * Calculates the polynomial hash value of a given string.
 *
 * @param str The input string to calculate the hash value for.
 * @param p The prime number used for hashing (default is 31).
 * @param m The modulo value used for hashing (default is 1e9 + 9).
 * @return The calculated hash value of the input string.
 */
long long polynomial_hash(const char *str, long long p = 31, long long m = 1e9 + 9)
{
    long long hash = 0;
    long long p_pow = 1;
    for (int i = 0; str[i] != '\0'; ++i)
    {
        hash = (hash + (str[i] - '0' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }

    return hash;
}

/**
 * Calculates the hash value of a given string using a polynomial hash function.
 *
 * @param str The string for which the hash value needs to be calculated.
 * @return The hash value of the string.
 */
long long get_hash(const String &str)
{
    return polynomial_hash(str.data);
}

/**
 * Calculates the hash value of the given integer.
 *
 * @param num The integer to calculate the hash value for.
 * @return The hash value of the given integer.
 */
long long get_hash(const int &num)
{
    return num;
}

/**
 * @brief A hash table implementation.
 *
 * @tparam Key The type of the keys stored in the hash table.
 * @tparam Value The type of the values stored in the hash table.
 */
template <typename Key, typename Value>
class HashTable
{
    /**
     * @brief A struct representing a node in the hash table.
     */
    struct Node
    {
        Key key;
        Value value;
        Node *next;
    };

    Node **table;
    int bucket_count;
    float load_factor = 0.7;

    /**
     * @brief Calculates the hash value for a given key.
     *
     * @param key The key for which to calculate the hash value.
     * @return The hash value.
     */
    int hash(Key key) const
    {
        return get_hash(key) % bucket_count;
    }

public:
    /**
     * @brief Constructs a new HashTable object.
     *
     * @param max_size The maximum number of elements the hash table can hold.
     */
    HashTable(int max_size)
    {
        bucket_count = max_size / load_factor;
        table = new Node *[bucket_count];
        for (int i = 0; i < bucket_count; ++i)
        {
            table[i] = nullptr;
        }
    }

    /**
     * @brief Destroys the HashTable object and frees the allocated memory.
     */
    ~HashTable()
    {
        for (int i = 0; i < bucket_count; ++i)
        {
            Node *node = table[i];
            while (node != nullptr)
            {
                Node *next = node->next;
                delete node;
                node = next;
            }
        }

        delete[] table;
    }

    /**
     * @brief Inserts a key-value pair into the hash table.
     *
     * @param key The key to insert.
     * @param value The value to insert.
     */
    void insert(Key key, Value value)
    {
        int index = hash(key);
        Node *node = table[index];
        while (node != nullptr)
        {
            if (node->key == key)
            {
                node->value = value;
                return;
            }

            node = node->next;
        }

        node = new Node();
        node->key = key;
        node->value = value;
        node->next = table[index];
        table[index] = node;
    }

    /**
     * @brief Finds the value associated with a given key in the hash table.
     *
     * @param key The key to search for.
     * @return A pointer to the value associated with the key, or nullptr if the key is not found.
     */
    Value *find(Key key) const
    {
        int index = hash(key);
        Node *node = table[index];
        while (node != nullptr)
        {
            if (node->key == key)
            {
                return &node->value;
            }

            node = node->next;
        }

        return nullptr;
    }

    /**
     * @brief Removes a key-value pair from the hash table.
     *
     * @param key The key to remove.
     */
    void erase(Key key)
    {
        int index = hash(key);
        Node *node = table[index];
        Node *prev = nullptr;
        while (node != nullptr)
        {
            if (node->key == key)
            {
                if (prev == nullptr)
                {
                    table[index] = node->next;
                }
                else
                {
                    prev->next = node->next;
                }

                delete node;
                return;
            }

            prev = node;
            node = node->next;
        }
    }
};

void solve()
{
    // -------------------------------- Fast IO -------------------------------
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    // ------------------------ Stream IO to Files ----------------------------
    (void)!freopen(INPUT_FILE, "r", stdin);
    (void)!freopen(OUTPUT_FILE, "w", stdout);

    // ------------------------- Setup Hash Tables ----------------------------
    HashTable<int, String> pseudonyms(MAX_REVIEWER_CNT * MAX_PSEUDONYM_CNT);
    HashTable<String, Reviewer> reviewers(MAX_REVIEWER_CNT);

    // ------------------------ Loop Through the File -------------------------
    char prefix;
    while (cin >> prefix)
    {
        // ------------------------------ Insert ------------------------------
        if (prefix == 'I')
        {
            String name;
            int new_pseudonym_count, new_pseudonyms[MAX_PSEUDONYM_CNT];
            cin >> name.data >> new_pseudonym_count;
            for (int i = 0; i < new_pseudonym_count; ++i)
                cin >> new_pseudonyms[i];

            if (reviewers.find(name) == nullptr)
            {
                Reviewer reviewer(name);
                reviewers.insert(name, reviewer);
            }

            Reviewer &reviewer = *(reviewers.find(name));

            bool can_insert = true;
            for (int i = 0, cnt = 0; i < new_pseudonym_count; ++i)
            {
                if (pseudonyms.find(new_pseudonyms[i]) != nullptr)
                {
                    if (*(pseudonyms.find(new_pseudonyms[i])) != name)
                    {
                        can_insert = false;
                        break;
                    }
                }
                else
                {
                    ++cnt;
                }

                if (reviewer.pseudonym_count + cnt > MAX_PSEUDONYM_CNT)
                {
                    can_insert = false;
                    break;
                }
            }

            if (can_insert)
            {
                for (int i = 0; i < new_pseudonym_count; ++i)
                {
                    if (pseudonyms.find(new_pseudonyms[i]) == nullptr)
                    {
                        pseudonyms.insert(new_pseudonyms[i], name);
                        reviewer.pseudonyms[reviewer.pseudonym_count++] = new_pseudonyms[i];
                    }
                }

                cout << "ok" << endl;
            }
            else
            {
                cout << "no" << endl;
            }
        }
        // ------------------------------ Delete ------------------------------
        else if (prefix == 'D')
        {
            int pseudonym;
            cin >> pseudonym;

            if (pseudonyms.find(pseudonym) != nullptr)
            {
                String name = *(pseudonyms.find(pseudonym));
                Reviewer &reviewer = *(reviewers.find(name));

                for (int i = 0; i < reviewer.pseudonym_count; ++i)
                {
                    pseudonyms.erase(reviewer.pseudonyms[i]);
                }

                reviewers.erase(name);

                cout << "ok" << endl;
            }
            else
            {
                cout << "no" << endl;
            }
        }
        // ------------------------------ Lookup ------------------------------
        else if (prefix == 'L')
        {
            int pseudonym;
            cin >> pseudonym;

            if (pseudonyms.find(pseudonym) != nullptr)
                cout << pseudonyms.find(pseudonym)->data << endl;
            else
                cout << "no" << endl;
        }
    }
}

int main()
{
    // ----------------------------- Measure Time -----------------------------
    auto start = std::chrono::high_resolution_clock::now();

    solve();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // cout << "Time in miliseconds: " << duration.count() << endl;

    return 0;
}