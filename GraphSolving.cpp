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
    FAST;
    ifstream cin("input.txt");
    ofstream cout("output.txt");
    ll n, m;
    cin >> n >> m;
    // n : nodes
    // m : edges
    vector<vector<vector<ll>>>adj(n, vector<vector<ll>>(2));
    while (m--)
    {
        ll u, v;
        cin >> u >> v;
        adj[u][0].pb(v);
        adj[v][1].pb(u);
    }
    vdll dist(n, vll(2, inf)); // dist[node][parity]
    // We will use djikstra's algorithm  
    priority_queue<array<ll, 3>, vector<array<ll, 3>>, greater<array<ll, 3>>>pq;
    dist[0][0] = 0;
    pq.push({ 0,0,0 });
    //arr[0]=dist, arr[1]= node, arr[2]=parity
    while (!pq.empty())
    {
        array<ll, 3>top = pq.top();
        pq.pop();
        ll node = top[1];
        ll parity = top[2];
        ll d = top[0];
        for (auto neighbours : adj[node][parity])
        {
            if (dist[neighbours][parity] > d + 1)
            {
                dist[neighbours][parity] = d + 1;
                pq.push({ d + 1,neighbours,parity });
            }
        }


        if (dist[node][1 ^ parity] > d + n)
        {
            dist[node][1 ^ parity] = d + n;
            pq.push({d + n,node,parity ^ 1 });

        }

    }
    //final state: unflipped
    ll cost1 = dist[n - 1][0];
    //final state : flipped
    ll cost2 = dist[n - 1][1];
    ll min_cost = min(cost1, cost2);
    if (min_cost >= inf)
    {
        // cout << "FAILED" << endl;
        cout<<-1<<endl;
    }
    else
    {
        // cout << "SUCCESS" << endl;
        cout<< min_cost << endl;
    }
}

/*
Explanation:
After extracting the graph information from the nodes.
We will use Djikstra's algorithm on this grpah.
Though the graph isn't weighted. We will see how its actually kinda virtually weighted
Firstly we defined a 3D vector, where  adj[node][parity] has 
all other neighbours of node when the situation is wrt parity
parity 0: unflipped version
parity 1: flipped version
We use parity because there are two states, either our graph is the one given to us, or it is flipped
Lets say we find the best answer to reach the target base N-1 in non-flipped state, and also in flipped state
Our final minimum cost would be the minimum of these answers.
If we think the same for any node, and find the best answers for each node,
we would end up reaching N-1 base.
 --> When cant we reach when bases 0 and N-1 are not at all connected with any means
 --> Except this case we can always flip each time we face any reversed edge and reach, 
    that means we just have to  make our answer better
 --> Consider a node* wrt a node, a node to node* (flipped version) takes N cost, 
    what if we see it as an edge and we are going from node --> node* in N cost? 
    and for all node we will have  new neighbour node*.
--> Thus we have a weighted edge, for all nodes in unflipped version, we can go to there neighbours
    in cost 1. And node* neighbour would be reached in cost N. 
    The same could be said wrt node* as node**=node itself.
    and adj[node][1] denotes the flipped adjancenty vector
--> We use the djikstra algorithm with a min-heap (priority_queue with a greater<>)
--> We use minheap with array<ll,3> where arr[]={dist, node, parity}
--> we first look on the routine neighbours which are in same parity and push them in pq
--> then we have a look up towards our node*, the alternate version of node, there we flip the parity and 
    push the 1^parity (1 xor parity) and thr required things in the min-heap. (As this node belongs to 1^parity version)
--> We keep pushing until our pq is empty
--> We were maintaining a dist[node][parity] which refers to the smallest distance discovered till the point we have traversed
    from 0,0 state to reach node,parity state
--> Therefore at last there are two possible candidates for the cost,
    * Minimum cost to reach N-1 base in flipped version which is dist[n-1][1]
    * Minimum cost to reach N-1 base in unflipped version which is dist[n-1][0]
--> Taking the minimum of them gives us the best answer, therefore
    our answer is min(dist[n-1][0], dist[n-1][1]).
--> Thanks for the read :)

*/
