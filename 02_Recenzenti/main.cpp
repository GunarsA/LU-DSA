#include <iostream>

using namespace std;

#define MAX_NAME_LEN 100
#define MAX_PSEUDONYM_CNT 50
#define MAX_REVIEWER_CNT 10000

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

struct Reviewer
{
    String name;
    int pseudonym_count;
    int pseudonyms[MAX_PSEUDONYM_CNT];

    Reviewer() {}

    Reviewer(String name)
    {
        this->name = name;
        pseudonym_count = 0;
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
    int size;

    int hash(Key key) const
    {
        return get_hash(key) % size;
    }

public:
    HashTable(int bucket_count)
    {
        this->size = bucket_count;
        table = new Node *[bucket_count];
        for (int i = 0; i < bucket_count; ++i)
        {
            table[i] = nullptr;
        }
    }

    ~HashTable()
    {
        for (int i = 0; i < size; ++i)
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

    Value *find(Key key)
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
};

int main()
{
    (void)!freopen("reviewers.in", "r", stdin);
    (void)!freopen("reviewers.out", "w", stdout);

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
            }

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
                    if (*(pseudonyms.find(new_pseudonyms[i])) == name)
                    {
                        ++cnt;
                    }
                    else
                    {
                        can_insert = false;
                        break;
                    }
                }

                if (reviewer.pseudonym_count + cnt >= MAX_PSEUDONYM_CNT)
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

    return 0;
}