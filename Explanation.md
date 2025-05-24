# Documentation

## Overview
Firstly we extract a adjacency list from the input image. Rename the file containing the adjacency list as `adj_list.txt`.
The adj_list.txt is in similar format as below:
```
0 0 1 .... 0 1
0 1 0 .... 0 1
.            .
.            .
.            .
0 0 1 .... 1 0
```

Run the code in `adj_list_to_input_form.cpp`, it takes input from `adj_list.txt` in same folder and it will output the required format in a `input.txt` file, 
which will have information about the graph in the following format:

$$
\begin{aligned}
N \quad &E \\
u_1 \quad &v_1 \\
u_2 \quad &v_2 \\
u_3 \quad &v_3 \\
\vdots \quad &\vdots \\
u_E \quad &v_E
\end{aligned}
$$

where $N$ is total nodes, $E$ is total number of `directed` edges, then $E$ line follows,
The line contains two nodes $u_i$ and $v_i$ if there exists a directed edge from $u_i$ to $v_i$
Formally,
We have a directed graph $G=(V,E)$ with $N$ nodes numbered from $0$ to $N-1$ extracted from `input image`.



Phantom Unit‑1 starts at node 0 (Base 0) and must reach node $N-1$ (Base  $N-1$ ).  The unit has two possible moves:

1. **Traverse Route**: If there is a directed edge $v \to u$ in the current orientation, move from $v$ to $u$ at cost $1$.
2. **Reverse Signals (Global Flip)**: Reverse the direction of *all* edges simultaneously at cost $N$: every edge $v\to u$ becomes $u\to v$.

We wish to compute the minimum total fuel cost to reach Base  $N-1$  from Base 0, or report $-1$ if it is impossible.

---

# Parity based Graph Model

To handle the all-flips quite efficiently, we construct a parity based graph with two 'versions' of a node:

* **Parity 0**: original graph $G$.
* **Parity 1**: reversed graph $G_{1}$.


These are the only possible states, because if the total number of $Reverse\\;Signals$ used is even, then we have our current graph as $G$, if not then the graph is $G_1$.
Formally

If $\text{flip} \equiv 1 \pmod{2}$, then the graph is $G_1$.  
If $\text{flip} \equiv 0 \pmod{2}$, then the graph is $G$.

Each state is a pair $(v,p)$ where $v\in V$ and $p\in{0,1}$.  Thus we have $2N$ states.
In some sense, we could say that each node has another version of it.
Though the graph is `unweighted`, we could say that all true neighbours w.r.t some node `v` are at distance 1, the alternate version $v^{*}$ is at a distance $N$ from $v$.
Formally speaking, these are the `transitions`.


# Transitions

1. **Traverse in same parity** $(v,p) \to (u,p)$ if in parity(version) $p$, there is an edge $v \to u$.  Cost  $=1$.
2. **Global flip** $(v,p) \to (v,1-p)$.  Cost  $=N$ . 
    Alias: $v \to v^{*}$


This kind of yields a weighted directed graph on $2N$ nodes with two possible edge weights ${1,N}$.


---

## Notations

* **adj**: a adj matrix (description in next section)
* **dist**: an $N\times 2$ array where $\mathrm{dist}[v][p]$ is the best known cost to reach $(v,p)$.  Initialized to $\infty$, except $\mathrm{dist}[0][0]=0$.
* **Min‐heap**: priority queue storing triples $(d,v,p)$, ordered by $d$.

**What is Dijkstra's Algorithm?**

Dijkstra's algorithm finds the shortest paths from a single source node to all other nodes in a graph with **non-negative edge weights**. It works by repeatedly selecting the unsettled node with the smallest tentative distance and relaxing its outgoing edges.

* **Why use a priority queue?**

  * We need to quickly extract the node with the minimum tentative distance at each step.
  * A min-heap (`priority_queue` with `greater<>`) achieves this in O(log V) time per extraction, leading to overall O((E+V) log V) complexity.

* **Key steps:**

  1. **Initialization**:

     ```cpp
     for (int v = 0; v < N; ++v) dist[v] = INF;
     dist[source] = 0;
     pq.push({0, source});  // {distance, node}
     ```
  2. **Pushing other nodes in the priority queue**:

     ```cpp
     while (!pq.empty()) {
         auto [d, u] = pq.top(); pq.pop();
         if (d > dist[u]) continue; 

         for (auto &edge : adj[u]) {
             int v = edge.first;
             ll w = edge.second;
             if (dist[v] > d + w) {
                 dist[v] = d + w;
                 pq.push({dist[v], v});
             }
         }
     }
     ```

* **Important considerations:**

  * All weights w must be >= 0.
  * The priority queue in its min heap version is to be used, and the first entry in pair has to be `distance` not the `source`, since we always want the least distances first.
  * The `if (d > dist[u]) continue;` check skips outdated entries, ensuring correctness and efficiency, though it is not really necessary but helps us not do any unnecessary checks.
  * Complexity: O((E+V) log V) with a binary heap.
  * We could use queue, but its not optimal for higher values of V or E, as the complexity is obviously higher than when we use a min heap


# Dijkstra’s Algorithm in our Parity- based graph
How is this graph algorithm help us in finding minimum fuel cost? Lets have a look at it!
Since all edge weights are positive, we can run Dijkstra’s algorithm on this $2N$-node graph to find the shortest paths from $(0,0)$ to both $(N-1,0)$ and $(N-1,1)$.  The answer is
$\min\bigl(\mathrm{dist}[(N-1,0)],\,\mathrm{dist}[(N-1,1)]\bigr),$
or $-1$ if both are infinite.
We use a 3D `adj` vector in the code, where 
1. `adj[v][0]` refer to all neighbours from node v in the parity 0 version.
2. `adj[v][1]` refer to all neighbours from node v in the parity 1 version.

For any two nodes belonging to different parity, say $v$ and $u^{*}$ among the total $2N$ nodes, they are neighbours only when they are the same node i.e  $v=u$

# More on why this works?

Lets look it from the start, we had `N` nodes and M directional `edges`, now these `M` edges have weight 1. 
The graph in its true sense is unweighted.
Lets also have a look on $G_1$ the reversed version or the flipped version of $G$. 
It also has `M` directional edges with each edge reversed in direction.
A thing to note is in these M edges do include bidirectional edges as a $2$.
Now in simple terms, what are the neighbours of a node $v$, they simply are:
1. All the neighbours in the input through directional edges ( `weight` = $1$ )
2. the node $v^{*}$ ( `weight` = $N$ )

Now we have it quite sorted, there are $2N$ nodes, we have to reach base $N-1$. Its exactly a set-up where one would like to use the above mentioned 
Djikstra's algorithm and even the weights are `non-negative`, and thus find the shortest path on this new `weighted graph` of `2N` nodes.
The code explanation is in comments in `Graph_Solving.cpp` and also after the `code block`, which explains how the code executes the same with the clever use of `parity` and vectors defined on the basis of `parity`.
I have tried the best to not write any repetettive line of `code` and thus have cleverly dealt with the `binary` cases using the concept of parity and flipping it for the next case.












## Pseudocode

```text
For safe reasons we can take +∞ as around 1e15.

Initialize dist[v][p] = +∞ for all v∈[0..N−1], p∈{0,1}
dist[0][0] = 0
push (0,0,0) into min‐heap  // (cost/dist, node, parity)

while min-heap is not empty:
    (d,v,p) = pop min‐heap

    // 1) traverse cost=1
    for u in adj[v][p]:
        if dist[u][p] > d+1:
            dist[u][p] = d+1
            push (d+1, u, p)

    // 2) flip cost=N or say traversing acroos the versions
    np = 1 ^ p
    if dist[v][np] > d+N:
        dist[v][np] = d+N
        push (d+N, v, np)

answer = min(dist[N−1][0], dist[N−1][1])
if answer >= +∞: output −1
else: output answer
```

---

# Complexity

Let $M$ be total number of edges. In the parity-based graph there are $2N$ vertices and at most $M$ edges of weight 1 and there are $2N$ flip‐edges of weight $N$.  Running Dijkstra with a binary heap costs
$O\bigl((M+N)\log (N)\bigr),$
which is efficient for large $N,M$ (e.g.  $N\le10^6$ & $M\le10^6$ ).
($2N$ flip-edges due to there is a bidirectional edge between $u$ and $u^{*}$)


# C++ Implementation Highlights

```cpp
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


```
## Code Explaination

After extracting the graph information from the nodes, we will use Dijkstra's algorithm on this graph. Though the graph isn't weighted, we see how it's virtually weighted:

* We define a 2D vector `adj[node][parity]`, where:

  * `parity = 0`: unflipped version
  * `parity = 1`: flipped version

* Parity captures the two states: either the graph is as given, or fully flipped.

* We find the best cost to reach base **N−1** in both the non-flipped and flipped states.

* Our final minimum fuel cost is
  \$\displaystyle \min\bigl(\text{dist}\[N-1]\[0],,\text{dist}\[N-1]\[1]\bigr)\$.

* If bases 0 and N−1 aren't connected (even undirected), it's impossible. Otherwise, by flipping when necessary, we can always find a path and then improve the answer.

* **Key idea:** Treat a flip from `node` → `node*` (the same node in the opposite parity) as an edge of weight **N**, and normal moves `node` → `neighbor` as edges of weight **1**.

* `adj[node][1]` holds the flipped adjacency list.

* Use Dijkstra's algorithm with a min-heap (`priority_queue` with `greater<>`).

  * Heap entries are arrays: `{dist, node, parity}`.
  * First relax all same-parity neighbors with cost +1.
  * Then relax the flip edge to `(node, 1^parity)` with cost +N.
  * Repeat until the heap is empty.

* We maintain `dist[node][parity]` as the smallest discovered cost from `(0,0)` to `(node, parity)`.

* At the end, two candidates remain for base `N-1`:

  * `dist[N-1][0]` (unflipped)
  * `dist[N-1][1]` (flipped)

* Taking the minimum of these gives the optimal fuel cost.

*Thanks for the read!*


---

# Usage
Note: The required code is in GraphSolving.cpp and reads input from input.txt and outputs in output.txt

1. reads integers `N` and `M`.
2. Read `M` pairs `u v` (0‐based nodes). (In different lines)
3. Runs the Djikstra's Algorithm in the parity based $2N$ Nodes graph.
4. Print the minimum fuel to reach base  $N-1$  or `-1` if unreachable.
5. Output is in a file output.txt in the same folder where you would be running your file locally in.
---
# FINISH! I sincerely hope you enjoyed reading it~



