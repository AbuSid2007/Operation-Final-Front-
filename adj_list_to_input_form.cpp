#include <bits/stdc++.h>
using namespace std;
#define pll pair<ll,ll>
#define ll long long
#define ld long double
#define vdll vector<vll>
#define vpll vector<pair<ll,ll>>
#define vll vector<ll>
#define all(x) x.begin(),x.end()
#define eb emplace_back
#define FAST ios::sync_with_stdio(false); cin.tie(nullptr);
#define pb push_back
#define inf 1e15

signed main() 
{
    // Read the adj_list and convert it into a feasible input form for 
    // GraphSolving.cpp file
    ifstream fin("adj_list.txt");
    ofstream fout("input.txt");
    vdll adj;
    string line;

    while (getline(fin, line)) 
    {
        istringstream iss(line);
        vll row;
        ll val;
        while (iss >> val) 
        {
            row.push_back(val);
        }
        adj.push_back(row);
    }
    int N = adj.size();
    vpll edges; //pair of vectors

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < adj[i].size(); j++) 
        {
            if (adj[i][j] == 1) 
            {
                edges.eb(i, j);
            }
        }
    }
    
    fout << N << " " << edges.size() << "\n";
    for (auto& [u, v] : edges) 
    {
        fout << u << " " << v << "\n";
    }
}
