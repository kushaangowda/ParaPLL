#include <bits/stdc++.h>
#include <semaphore.h>

#define inf INT_MAX
#define fr(i,a,b) for(int i=a;i<b;i++)

using namespace std;

typedef pair<int, int> pi;

vector<vector<int>> L, G;
queue<int> Q;

int numVertices;

sem_t q, l;

int Query(int s, int t, vector<vector<int>> L){
    int ans = inf;
    fr(i,0,L.size()) {
        if(L[s][i] != inf && L[t][i] != inf) ans = min(ans, L[s][i]+L[t][i]);
    }
    return ans;
}

void pruned_dijkstra(int v){
    int numVertices = G.size();
    vector<int> D;

    cout<<endl;
    cout<<"Vertex "<<v<<endl;

    fr(i,0,numVertices) D.push_back(inf);
    D[v] = 0;

    priority_queue<pi,vector<pi>,greater<pi>> Q;
    Q.push(make_pair(0,v));

    while(!Q.empty()){
        cout<<Q.top().first<<" "<<Q.top().second<<endl;
        int u = Q.top().second;
        Q.pop();
        
        sem_wait(&l);
        if(Query(v,u,L) <= D[u]){
            sem_post(&l);
            continue;
        }
        L[u][v] = L[v][u] = D[u];
        sem_post(&l);

        fr(i,0,numVertices){
            if(G[u][i] != inf && D[i] == inf){
                D[i] = D[u] + G[u][i];
                Q.push(make_pair(D[i],i));
            }
        }        

    }
    cout<<endl;
}

vector<pi> getOutDegree(){
    vector<pi> ans;
    fr(i,0,numVertices){
        int count = 0;
        fr(j,0,numVertices){
            if(G[i][j] != inf) count++;
        }
        ans.push_back(make_pair(count,i));
    }
    return ans;
}

vector<vector<int>> getGraph(){
    vector<vector<int>> G;

    vector<tuple<int, int, int>> edges;
    int ne,nv;

    cout<<"Num of vertices: ";
    cin>>nv;
    cout<<"Num of edges: ";
    cin>>ne;
    
    vector<int> v;
    fr(i,0,nv) v.push_back(inf);
    fr(i,0,nv) G.push_back(v);

    fr(i,0,ne){
        int a,b,c;
        cin>>a>>b>>c;
        G[a][b] = G[b][a] = c;
    }

    return G;
}

void initialize(){
    G = getGraph();
    numVertices = G.size();
    
    vector<int> v;
    fr(j,0,numVertices) v.push_back(inf);
    fr(i,0,numVertices) L.push_back(v);

    vector<pi> outDegreee = getOutDegree();
    sort(outDegreee.rbegin(), outDegreee.rend());

    for(auto x:outDegreee) Q.push(x.second);
}

void algo2(){
    while(!Q.empty()){
        sem_wait(&q);
        int v = Q.front();
        Q.pop();
        sem_post(&q);

        pruned_dijkstra(v);
    }
}

int main(){
    sem_init(&q,0,1);
    sem_init(&l,0,1);

    initialize();

    algo2();

    cout<<"\nL:\n";
    fr(i,0,L.size()){
        fr(j,0,L[i].size()){
            if(L[i][j] != inf){
                cout<<"L["<<i<<"]["<<j<<"] = "<<L[i][j]<<endl;
            }
        }
    }

    cout<<Query(0,3,L)<<endl;

    return 0;
}

// g++ algo2.cpp -lpthread