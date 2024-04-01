#include <iostream>

#include <map>

using namespace std;

#define MAX_PSEUDONYM_CNT 50

struct Reviewer
{
    string name;
    int pseudonym_count;
    int pseudonyms[MAX_PSEUDONYM_CNT];

    Reviewer() {}

    Reviewer(string name) : name(name), pseudonym_count(0) {}
};

int main()
{
    (void)!freopen("reviewers.in", "r", stdin);
    (void)!freopen("reviewers.out", "w", stdout);

    map<int, string> pseudonyms;
    map<string, Reviewer> reviewers;

    char prefix;
    while (cin >> prefix)
    {
        if (prefix == 'I')
        {
            string name;
            int new_pseudonym_count, new_pseudonyms[MAX_PSEUDONYM_CNT];
            cin >> name >> new_pseudonym_count;
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
                    if (pseudonyms[new_pseudonyms[i]] == name)
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
                    if(pseudonyms.find(new_pseudonyms[i]) == pseudonyms.end())
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
                string name = pseudonyms[pseudonym];
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
                cout << pseudonyms[pseudonym] << endl;
            }
            else
            {
                cout << "no" << endl;
            }
        }
    }

    return 0;
}