#include <iostream>

#include <chrono>

using namespace std;

#define MAX_NAME_LEN 100
#define MAX_PSEUDONYM_CNT 50
#define MAX_REVIEWER_CNT 10000

#define INPUT_FILE "reviews.in"
#define OUTPUT_FILE "reviewers.out"

struct String
{
    char data[MAX_NAME_LEN];

    String() {}

    String(char data[])
    {
        for (int i = 0; i < MAX_NAME_LEN; ++i)
        {
            this->data[i] = data[i];
            if (data[i] == '\0')
            {
                break;
            }
        }
    }

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

    bool operator!=(const String &other) const
    {
        return !(*this == other);
    }

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

    bool operator<=(const String &other) const
    {
        return *this < other || *this == other;
    }

    bool operator>=(const String &other) const
    {
        return *this > other || *this == other;
    }
};

template <typename T>
class Array
{
private:
    T data[MAX_PSEUDONYM_CNT];
    int size = MAX_PSEUDONYM_CNT;

public:
    T &operator[](int index)
    {
        if (index >= 0 && index < size)
            return data[index];
        else
            throw "Index out of bounds";
    }
};

struct Reviewer
{
    String name;
    int pseudonym_count;
    Array<int> pseudonyms;

    Reviewer() {}

    Reviewer(String name)
    {
        this->name = name;
        pseudonym_count = 0;
    }

    bool operator==(const Reviewer &other) const
    {
        return name == other.name;
    }
};

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

long long get_hash(const String &str)
{
    return polynomial_hash(str.data);
}

long long get_hash(const int &num)
{
    return num;
}

template <typename Key, typename Value>
class HashTable
{
    struct Node
    {
        Key key;
        Value value;
        Node *next;
    };

    Node **table;
    int bucket_count;
    float load_factor = 0.7;

    int hash(Key key) const
    {
        return get_hash(key) % bucket_count;
    }

public:
    HashTable(int max_size)
    {
        bucket_count = max_size / load_factor;
        table = new Node *[bucket_count];
        for (int i = 0; i < bucket_count; ++i)
        {
            table[i] = nullptr;
        }
    }

    ~HashTable()
    {
        for (int i = 0; i < bucket_count; ++i)
        {
            Node *node = table[i];
            while (node != nullptr)
            {
                Node *next = node->next;
                cout << 1 << " ";
                delete node;
                node = next;
            }
            cout << endl;
            if (i % 10000 == 0)
            {
                cout << i << endl;
            }
        }

        delete[] table;
    }

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

    // void print() const
    // {
    //     for (int i = 0; i < bucket_count; ++i)
    //     {
    //         Node *node = table[i];
    //         while (node != nullptr)
    //         {
    //             cout << 1 << " ";
    //         }
    //         cout << endl;
    //     }
    // }
};

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    ios_base::sync_with_stdio(false);
    cin.tie(0);

    (void)!freopen(INPUT_FILE, "r", stdin);
    (void)!freopen(OUTPUT_FILE, "w", stdout);

    HashTable<int, String> pseudonyms(MAX_REVIEWER_CNT * MAX_PSEUDONYM_CNT);
    HashTable<String, Reviewer> reviewers(MAX_REVIEWER_CNT);

    char prefix;
    while (cin >> prefix)
    {
        if (prefix == 'I')
        {
            String name;
            int new_pseudonym_count, new_pseudonyms[MAX_PSEUDONYM_CNT];
            cin >> name.data >> new_pseudonym_count;
            for (int i = 0; i < new_pseudonym_count; ++i)
            {
                cin >> new_pseudonyms[i];

                // cout << new_pseudonyms[i] << " ";
            }
            // cout << endl;

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

            // cout << reviewer.pseudonym_count << endl;
        }
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
        else if (prefix == 'L')
        {
            int pseudonym;
            cin >> pseudonym;

            if (pseudonyms.find(pseudonym) != nullptr)
            {
                cout << pseudonyms.find(pseudonym)->data << endl;
            }
            else
            {
                cout << "no" << endl;
            }
        }
    }

    // for (int i = 1; i <= 10000; i++)
    // {
    //     if (pseudonyms.find(i) != nullptr)
    //         cout << i << " " << pseudonyms.find(i)->data << endl;
    // }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // cout << "Time in miliseconds: " << duration.count() << endl;

    return 0;
}