#include <iostream>

#include <map>

using namespace std;

#define MAX_NAME_LEN 100
#define MAX_PSEUDONYM_CNT 50

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

int main()
{
    (void)!freopen("1.in", "r", stdin);
    (void)!freopen("reviewers.out", "w", stdout);

    map<int, String> pseudonyms;
    map<String, Reviewer> reviewers;

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

            if (reviewers.find(name) == reviewers.end())
            {
                Reviewer reviewer(name);
                reviewers.insert({name, reviewer});
            }

            Reviewer &reviewer = reviewers[name];

            bool can_insert = true;
            for (int i = 0, cnt = 0; i < new_pseudonym_count; ++i)
            {
                if (pseudonyms.find(new_pseudonyms[i]) != pseudonyms.end())
                {
                    if (pseudonyms[new_pseudonyms[i]] != name)
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
                    if (pseudonyms.find(new_pseudonyms[i]) == pseudonyms.end())
                    {
                        pseudonyms.insert({new_pseudonyms[i], name});
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

            if (pseudonyms.find(pseudonym) != pseudonyms.end())
            {
                String name = pseudonyms[pseudonym];
                Reviewer &reviewer = reviewers[name];

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

            if (pseudonyms.find(pseudonym) != pseudonyms.end())
            {
                cout << pseudonyms[pseudonym].data << endl;
            }
            else
            {
                cout << "no" << endl;
            }
        }
    }

    return 0;
}