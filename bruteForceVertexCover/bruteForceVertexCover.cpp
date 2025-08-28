#include <bits/stdc++.h>
using namespace std;

// Function to generate a random graph with given vertices and edges
vector<pair<int, int>> generate_graph(int vertices, int edges_count)
{
    set<pair<int, int>> edge;
    vector<pair<int, int>> all_possible_edges;

    for (int i = 0; i < vertices; ++i)
    {
        for (int j = i + 1; j < vertices; ++j)
        {
            all_possible_edges.emplace_back(i, j);
        }
    }

    shuffle(all_possible_edges.begin(), all_possible_edges.end(), default_random_engine());

    for (int i = 0; i < edges_count && i < (int)all_possible_edges.size(); ++i)
    {
        edge.insert(all_possible_edges[i]);
    }

    return vector<pair<int, int>>(edge.begin(), edge.end());
}

// Function to check if a subset of vertices is a valid vertex cover
bool is_vertex_cover(const set<int> &subset, const vector<pair<int, int>> &edges)
{
    for (const auto &edge : edges)
    {
        if (subset.find(edge.first) == subset.end() && subset.find(edge.second) == subset.end())
        {
            return false;
        }
    }
    return true;
}

// Brute-force algorithm to find minimum vertex cover
int brute_force_vertex_cover(int vertices, const vector<pair<int, int>> &edges)
{
    // for (int r = 1; r <= vertices; ++r)
    // { // Try subsets of increasing size
    //     vector<int> subset(r);
    //     iota(subset.begin(), subset.end(), 0);
    //     do
    //     {
    //         set<int> current_subset(subset.begin(), subset.end());
    //         if (is_vertex_cover(current_subset, edges))
    //         {
    //             return current_subset.size();
    //         }
    //     } while (next_permutation(subset.begin(), subset.end()));
    // }
    // return vertices;

    // Brute-force algorithm to find minimum vertex cover (fixed)
    // Uses bitmask enumeration and pruning
    int min_cover = vertices;

    // for vertices > 31 you must use 1ULL << vertices and unsigned long long masks
    int total_masks = 1 << vertices; // safe for vertices <= 30

    for (int mask = 1; mask < total_masks; ++mask)
    {
        int sz = __builtin_popcount(mask); // number of vertices in this subset
        if (sz >= min_cover)               // prune: no need to check larger/equal subsets
            continue;

        bool ok = true;
        for (const auto &edge : edges)
        {
            int u = edge.first;
            int v = edge.second;
            // check if neither endpoint is in the subset
            if (!(mask & (1 << u)) && !(mask & (1 << v)))
            {
                ok = false;
                break;
            }
        }

        if (ok)
            min_cover = sz;
    }

    return min_cover;
}

int main()
{
    // ---------- Step 1: Read input ----------
    ifstream infile("input.txt");
    if (!infile.is_open())
    {
        cout << "Error: input.txt not found." << endl;
        return 1;
    }

    int vertices;
    string line;

    getline(infile, line);
    vertices = stoi(line);

    getline(infile, line);
    vector<int> edges_values;
    stringstream ss(line);
    string token;
    while (getline(ss, token, ','))
    {
        edges_values.push_back(stoi(token));
    }
    infile.close();

    // ---------- Step 2: Prepare outputs ----------
    ofstream txtfile("results.txt");
    ofstream csvfile("results.csv");

    txtfile << "Vertex Cover Results\n";
    txtfile << "====================\n\n";

    csvfile << "Vertices, Edges, Vertex Cover Size, Running Time(s)\n";

    // ---------- Step 3: Run experiments ----------
    for (int edges_count : edges_values)
    {
        cout << "\nRunning for vertices=" << vertices << ", edges=" << edges_count << " ...\n";

        auto edges = generate_graph(vertices, edges_count);

        auto start_time = chrono::high_resolution_clock::now();
        int cover_size = brute_force_vertex_cover(vertices, edges);
        auto end_time = chrono::high_resolution_clock::now();

        chrono::duration<double> run_time = end_time - start_time;

        // Console
        cout << " -> Vertex Cover Size = " << cover_size
             << ", Time = " << run_time.count() << " sec\n";

        // Text file
        txtfile << "vertices=" << vertices
                << ", edges=" << edges_count
                << " -> Vertex Cover Size = " << cover_size
                << ", Time = " << run_time.count() << " sec\n";

        // CSV file
        csvfile << vertices << ","
                << edges_count << ","
                << cover_size << ","
                << run_time.count() << "\n";
    }

    txtfile.close();
    csvfile.close();

    cout << "\nResults saved to results.txt and results.csv\n";

    return 0;
}
