#include <bits/stdc++.h>
#include <semaphore.h>

#define inf INT_MAX
#define fr(i,a,b) for(int i=a;i<b;i++)

using namespace std;

typedef pair<int, int> pi;

int Query(int s, int t, vector<vector<int>> L){
    int ans = inf;
    fr(i,0,L.size()) {
        if(L[s][i] != inf && L[t][i] != inf) ans = min(ans, L[s][i]+L[t][i]);
    }
    return ans;
}

vector<vector<int>> pruned_dijkstra(vector<vector<int>> G, int v, vector<vector<int>> L, sem_t l){
    int numVertices = G.size();
    vector<int> D;

    cout<<endl;
    cout<<"Vertex "<<v<<endl;

    vector<vector<int>> Lk;
    fr(i,0,L.size()) Lk.push_back(L[i]);

    fr(i,0,numVertices) D.push_back(inf);
    D[v] = 0;

    priority_queue<pi,vector<pi>,greater<pi>> Q;
    Q.push(make_pair(0,v));

    while(!Q.empty()){
        cout<<Q.top().first<<" "<<Q.top().second<<endl;
        int u = Q.top().second;
        Q.pop();
        if(Query(v,u,Lk) <= D[u]) continue;
        Lk[u][v] = Lk[v][u] = D[u];
        fr(i,0,numVertices){
            if(G[u][i] != inf && D[i] == inf){
                D[i] = D[u] + G[u][i];
                Q.push(make_pair(D[i],i));
            }
        }        

    }
    cout<<endl;
    
    return Lk;
}