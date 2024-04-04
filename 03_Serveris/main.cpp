#include <iostream>

#include <chrono>

using namespace std;

#define INPUT_FILE "server.in"
#define OUTPUT_FILE "server.out"

#define MAX_NODES 20000

// ------------------------------ Data Structures -----------------------------

/**
 * @brief A template class representing an array.
 *
 * This class provides functionality to create and manipulate an array of elements of type T.
 */
template <typename T>
class Array
{
private:
    T *data;  /**< Pointer to the array data */
    int size; /**< Size of the array */

public:
    /**
     * @brief Constructs an Array object with the specified size.
     *
     * @param size The size of the array.
     */
    Array(int size) : size(size)
    {
        data = new T[size];
    }

    /**
     * @brief Destroys the Array object and frees the allocated memory.
     */
    ~Array()
    {
        delete[] data;
    }

    /**
     * @brief Overloaded subscript operator for accessing elements of the array.
     *
     * @param index The index of the element to access.
     * @return T& A reference to the element at the specified index.
     */
    T &operator[](int index)
    {
        return data[index];
    }

    /**
     * @brief Overloaded const subscript operator for accessing elements of the array.
     *
     * @param index The index of the element to access.
     * @return const T& A const reference to the element at the specified index.
     */
    const T &operator[](int index) const
    {
        return data[index];
    }

    /**
     * @brief Gets the size of the array.
     *
     * @return int The size of the array.
     */
    int getSize() const
    {
        return size;
    }
};

/**
 * @brief A template class representing a linked list.
 *
 * @tparam T The type of data stored in the linked list.
 */
template <typename T>
class LinkedList
{
private:
    /**
     * @brief A struct representing a node in the linked list.
     */
    struct Node
    {
        T data;     /**< The data stored in the node. */
        Node *next; /**< Pointer to the next node in the list. */

        /**
         * @brief Constructs a new Node object.
         *
         * @param data The data to be stored in the node.
         * @param next Pointer to the next node in the list.
         */
        Node(const T &data, Node *next) : data(data), next(next) {}
    };

    Node *head; /**< Pointer to the head node of the list. */

public:
    /**
     * @brief Constructs a new LinkedList object.
     */
    LinkedList() : head(nullptr) {}

    /**
     * @brief Destroys the LinkedList object and frees the memory.
     */
    ~LinkedList()
    {
        while (head != nullptr)
        {
            Node *next = head->next;
            delete head;
            head = next;
        }
    }

    /**
     * @brief Adds a new element to the front of the list.
     *
     * @param data The data to be added.
     */
    void add(const T &data)
    {
        head = new Node(data, head);
    }

    /**
     * @brief An iterator class for iterating over the elements of the list.
     */
    class Iterator
    {
    private:
        Node *current; /**< Pointer to the current node. */

    public:
        /**
         * @brief Constructs a new Iterator object.
         *
         * @param current Pointer to the current node.
         */
        Iterator(Node *current) : current(current) {}

        /**
         * @brief Checks if the iterator is not equal to another iterator.
         *
         * @param other The other iterator to compare with.
         * @return true if the iterators are not equal, false otherwise.
         */
        bool operator!=(const Iterator &other) const
        {
            return current != other.current;
        }

        /**
         * @brief Returns a reference to the data at the current position.
         *
         * @return const T& A reference to the data at the current position.
         */
        const T &operator*() const
        {
            return current->data;
        }

        /**
         * @brief Moves the iterator to the next position.
         */
        void operator++()
        {
            current = current->next;
        }
    };

    /**
     * @brief Returns an iterator pointing to the first element of the list.
     *
     * @return Iterator An iterator pointing to the first element of the list.
     */
    Iterator begin() const
    {
        return Iterator(head);
    }

    /**
     * @brief Returns an iterator pointing to the end of the list.
     *
     * @return Iterator An iterator pointing to the end of the list.
     */
    Iterator end() const
    {
        return Iterator(nullptr);
    }
};

// ----------------------------- Utility Functions ----------------------------

/**
 * Returns the maximum value between two values.
 *
 * @tparam T The type of the values.
 * @param a The first value.
 * @param b The second value.
 * @return The maximum value between `a` and `b`.
 */
template <typename T>
T Max(T a, T b)
{
    return a > b ? a : b;
}

/**
 * Returns the minimum value between two values.
 *
 * @tparam T The type of the values.
 * @param a The first value.
 * @param b The second value.
 * @return The minimum value between `a` and `b`.
 */
template <typename T>
T Min(T a, T b)
{
    return a < b ? a : b;
}

// ------------------------------ Main Functions ------------------------------

/**
 * Performs a depth-first search as the first step to calculate the depth of each node in a graph.
 *
 * @param node The current node being visited.
 * @param parent The parent node of the current node.
 * @param adj_list The adjacency list representation of the graph.
 * @param depths An array to store the depths of each node.
 * @return The depth of the current node.
 */
int dfs1(int node,
         int parent,
         const Array<LinkedList<int>> &adj_list,
         Array<int> &depths)
{
    int depth = 0;

    for (int child : adj_list[node])
    {
        if (child == parent)
        {
            continue;
        }

        depth = Max(depth, dfs1(child, node, adj_list, depths) + 1);
    }

    depths[node] = depth;
    return depth;
}

/**
 * Performs a depth-first search as the second step to calculate the depth of each node in a graph.
 *
 * @param node The current node being visited.
 * @param parent The parent node of the current node.
 * @param parent_depth The depth of the parent node.
 * @param adj_list The adjacency list representation of the graph.
 * @param depths An array to store the depths of each node.
 */
void dfs2(int node,
          int parent,
          int parent_depth,
          const Array<LinkedList<int>> &adj_list,
          Array<int> &depths)
{
    depths[node] = Max(parent_depth, depths[node]);

    int max_depth = -2, second_max_depth = -2;

    for (int child : adj_list[node])
    {
        if (child == parent)
        {
            continue;
        }

        if (depths[child] > max_depth)
        {
            second_max_depth = max_depth;
            max_depth = depths[child];
        }
        else if (depths[child] > second_max_depth)
        {
            second_max_depth = depths[child];
        }
    }

    for (int child : adj_list[node])
    {
        if (child == parent)
        {
            continue;
        }

        int child_depth = parent_depth + 1;
        if (max_depth == depths[child])
        {
            child_depth = Max(child_depth, second_max_depth + 2);
        }
        else
        {
            child_depth = Max(child_depth, max_depth + 2);
        }

        dfs2(child, node, child_depth, adj_list, depths);
    }
}

void solve()
{
    // -------------------------------- Fast IO -------------------------------
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // ------------------------ Stream IO to Files ----------------------------
    (void)!freopen(INPUT_FILE, "r", stdin);
    (void)!freopen(OUTPUT_FILE, "w", stdout);
    if (freopen(INPUT_FILE, "r", stdin) == 0 || freopen(OUTPUT_FILE, "w", stdout) == 0)
    {
        cout << "Error with opening file[s]!" << endl;
    }

    // ----------------------------- Read Input ------------------------------
    int node_cnt;
    cin >> node_cnt;

    Array<LinkedList<int>> adj_list(node_cnt);

    for (int i = 0; i < node_cnt - 1; ++i)
    {
        int a, b;
        cin >> a >> b;

        adj_list[a - 1].add(b - 1);
        adj_list[b - 1].add(a - 1);
    }

    // ------------------------------ Calculate -------------------------------
    Array<int> depths(node_cnt);

    dfs1(0, -1, adj_list, depths);
    dfs2(0, -1, 0, adj_list, depths);

    // ----------------------------- Print Output -----------------------------
    int min_depth = depths[0];
    for (int i = 1; i < node_cnt; ++i)
    {
        min_depth = Min(min_depth, depths[i]);
    }

    cout << min_depth << endl;
    for (int i = 0; i < node_cnt; ++i)
    {
        if (depths[i] == min_depth)
        {
            cout << i + 1 << " ";
        }
    }
    cout << endl;
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