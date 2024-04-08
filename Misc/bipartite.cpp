#include <iostream>
#include <vector>

using namespace std;

enum color
{
    white,
    black
};

bool is_bipartite(vector<vector<int>> &adj_list, vector<int> &color, int curr)
{
    for (auto i : adj_list[curr])
    {
        if (color[i] == color[curr])
            return false;
        if (color[i] == -1)
        {
            color[i] = (color[curr] == white ? black : white);
            if (!is_bipartite(adj_list, color, i))
                return false;
        }
    }
    return true;
}

int main()
{
    vector<vector<int>> adj_list = {
        {3, 4},
        {2, 4},
        {1, 5},
        {1},
        {0, 1, 5},
        {2, 4}};
    vector<int> color(adj_list.size(), -1);
    color[0] = white;
    cout << (is_bipartite(adj_list, color, 0) ? "Yes" : "No") << endl;

    adj_list = {
        {1, 4},
        {0, 2, 4},
        {1, 4},
        {0, 4},
        {1, 2, 3}};
    color = vector<int>(adj_list.size(), -1);
    color[0] = white;
    cout << (is_bipartite(adj_list, color, 0) ? "Yes" : "No") << endl;

    return 0;
}