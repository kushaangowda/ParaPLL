#include <bits/stdc++.h>

#define inf INT_MAX
#define fr(i,a,b) for(int i=a;i<b;i++)

using namespace std;

typedef pair<int, int> pi;

int Query(int s, int t, vector<vector<int>> L){
    int ans = inf;
    fr(i,0,L.size()) {
        if(L[s][i] == inf || L[t][i] == inf) continue;
        ans = min(ans, L[s][i]+L[t][i]);
    }
    return ans;
}

vector<vector<int>> pruned_dijkstra(vector<vector<int>> G, int v, vector<vector<int>> L){
    int numVertices = G.size();
    vector<int> D;

    fr(i,0,numVertices) D.push_back(inf);
    D[v] = 0;

    priority_queue<pi,vector<pi>,greater<pi>> Q;
    Q.push(make_pair(0,v));

    while(!Q.empty()){
        int u = Q.top().second;
        Q.pop();
        if(Query(v,u,L) <= D[u]) continue;
        L[u][v] = D[v];
        fr(i,0,numVertices){
            if(G[u][i] != 0 && D[i] == inf){
                D[i] = D[u] + G[u][i];
                Q.push(make_pair(D[i],i));
            }
        }        

    }

    // fr(i,0,L.size()){
    //     fr(j,0,L[i].size()){
    //         cout<<L[i][j]<<" ";
    //     }
    //     cout<<"\n";
    // }
    
    return L;
}