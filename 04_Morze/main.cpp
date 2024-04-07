#include <iostream>

#include <chrono>

using namespace std;

#define INPUT_FILE "morse.in"
#define OUTPUT_FILE "morse.out"

#define MAX_SYMBOL_SIZE 6

// ------------------------------ Utility Classes -----------------------------

class String
/**
 * @brief Represents a string of characters.
 */
{
private:
    int size;
    static const int capacity = MAX_SYMBOL_SIZE + 10;
    char data[capacity];

public:
    /**
     * @brief Default constructor.
     */
    String() : size(0)
    {
        data[0] = '\0';
    }

    /**
     * @brief Constructor with a character array parameter.
     *
     * @param str The character array to initialize the string with.
     */
    String(const char str[]) : size(0)
    {
        for (int i = 0; str[i] != '\0'; ++i)
        {
            data[size++] = str[i];
        }

        data[size] = '\0';
    }

    /**
     * @brief Overloaded subscript operator.
     *
     * This function allows accessing the character at the specified index in the 'data' array.
     *
     * @param index The index of the character to access.
     * @return A reference to the character at the specified index.
     */
    char &operator[](int index)
    {
        return data[index];
    }

    /**
     * @brief Adds a character to the end of the data array.
     *
     * This function appends the given character to the end of the data array and updates the size accordingly.
     *
     * @param ch The character to be added.
     */
    void push_back(char ch)
    {
        data[size++] = ch;
        data[size] = '\0';
    }

    /**
     * @brief Clears the data array and sets the size to zero.
     */
    void clear()
    {
        size = 0;
        data[0] = '\0';
    }

    /**
     * @brief Returns the size of the string.
     *
     * @return The size of the string.
     */
    int length() const
    {
        return size;
    }

    /**
     * @brief Overloaded equality operator.
     *
     * @param other The string to compare with.
     * @return true if the strings are equal, false otherwise.
     */
    bool operator==(const String &other) const
    {
        for (int i = 0; i < capacity; ++i)
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
        for (int i = 0; i < capacity; ++i)
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
        for (int i = 0; i < capacity; ++i)
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

// -------------------------------- Structures --------------------------------

struct Symbol
/**
 * @brief Represents a symbol in Morse code.
 */
{
    String code;
    char symbol;

    Symbol() : code(), symbol('\0') {}

    Symbol(const char code[], char symbol) : code(code), symbol(symbol) {}
};

void solve()
{
    // ------------------------ Stream IO to Files ----------------------------
    (void)!freopen(INPUT_FILE, "r", stdin);
    (void)!freopen(OUTPUT_FILE, "w", stdout);
    if (freopen(INPUT_FILE, "r", stdin) == 0 || freopen(OUTPUT_FILE, "w", stdout) == 0)
    {
        cout << "Error with opening file[s]!" << endl;
    }

    // ------------------------- Initialize Symbol Table ----------------------
    Symbol symbols[] = {
        {".-", 'A'},
        {"-...", 'B'},
        {"-.-.", 'C'},
        {"-..", 'D'},
        {".", 'E'},
        {"..-.", 'F'},
        {"--.", 'G'},
        {"....", 'H'},
        {"..", 'I'},
        {".---", 'J'},
        {"-.-", 'K'},
        {".-..", 'L'},
        {"--", 'M'},
        {"-.", 'N'},
        {"---", 'O'},
        {".--.", 'P'},
        {"--.-", 'Q'},
        {".-.", 'R'},
        {"...", 'S'},
        {"-", 'T'},
        {"..-", 'U'},
        {"...-", 'V'},
        {".--", 'W'},
        {"-..-", 'X'},
        {"-.--", 'Y'},
        {"--..", 'Z'},
        {"-----", '0'},
        {".----", '1'},
        {"..---", '2'},
        {"...--", '3'},
        {"....-", '4'},
        {".....", '5'},
        {"-....", '6'},
        {"--...", '7'},
        {"---..", '8'},
        {"----.", '9'},
        {".-.-.-", '.'},
        {"--..--", ','},
        {"..--..", '?'},
        {"-....-", '-'},
        {"-..-.", '/'},
        {".----.", ' '}};

    // ---------------------------- Decode Morse ------------------------------
    String symbol_code;
    char ch;
    while (cin >> ch)
    {
        if (ch == '|')
        {
            bool found = false;
            if (symbol_code.length() <= 6)
            {
                for (Symbol symbol : symbols)
                {
                    if (symbol.code == symbol_code)
                    {
                        cout << symbol.symbol;
                        found = true;
                        break;
                    }
                }
            }

            if (!found && symbol_code.length())
            {
                cout << '!';
            }

            symbol_code.clear();
        }
        else if (symbol_code.length() <= 6)
        {
            symbol_code.push_back(ch);
        }
    }

    if (symbol_code.length())
    {
        cout << '!';
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