#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream fin("input.txt", ios::in | ios::binary);

    int inp;
    for(int i = 0; i < 8; i++)
    {   
        fin >> inp;
        cout << inp << endl;
    }

    cout << "---" << endl;

    fin.clear();

    streampos pos = fin.tellg();

    while (inp)
    {   
        fin >> inp;
        cout << inp << endl;
    }

    cout << "---" << endl;
    fin.clear();
    fin.seekg(pos);

    do
    {   
        fin >> inp;
        cout << inp << endl;
    } while(inp);

    fin.close();

    return 0;
}