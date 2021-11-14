#include <bits/stdc++.h>
#include <omp.h>

#define inf INT_MAX
#define fr(i,a,b) for(int i=a;i<b;i++)

using namespace std;

typedef pair<int, int> pi;

vector<vector<int>> L, G;
vector<int> Q;

int numVertices;

omp_lock_t l;

int Query(int s, int t){
    int ans = inf;
    if(L[s][t] != inf) return L[s][t];
    fr(i,0,L.size()) {
        if(L[s][i] != inf && L[t][i] != inf) ans = min(ans, L[s][i]+L[t][i]);
    }
    return ans;
}

void pruned_dijkstra(int v){
    int numVertices = G.size();
    vector<int> D;

    fr(i,0,numVertices) D.push_back(inf);
    D[v] = 0;

    priority_queue<pi,vector<pi>,greater<pi>> Q;
    Q.push(make_pair(0,v));

    while(!Q.empty()){
        int u = Q.top().second;
        Q.pop();

        omp_set_lock(&l);
        if(Query(v,u) <= D[u]){
            omp_unset_lock(&l);
            continue;
        }
        L[u][v] = L[v][u] = D[u];
        omp_unset_lock(&l);

        fr(i,0,numVertices){
            if(G[u][i] != inf && D[i]>D[u]+G[u][i]){
                D[i] = D[u] + G[u][i];
                Q.push(make_pair(D[i],i));
            }
        }

    }
}

void getQueueOutDegree(){
    vector<pi> ans;
    fr(i,0,numVertices){
        int count = 0;
        fr(j,0,numVertices){
            if(G[i][j] != inf) count++;
        }
        ans.push_back(make_pair(count,i));
    }
    sort(ans.rbegin(), ans.rend());
    for(auto x:ans) Q.push_back(x.second);
}

void getGraphAndL(){
    vector<tuple<int, int, int>> edges;
    int ne;

    cin>>numVertices;
    cin>>ne;
    cout<<"Num of vertices: "<<numVertices<<endl;
    cout<<"Num of edges: "<<ne<<endl;
    
    vector<int> v;
    fr(i,0,numVertices) v.push_back(inf);
    fr(i,0,numVertices) G.push_back(v);
    fr(i,0,numVertices) L.push_back(v);

    fr(i,0,ne){
        int a,b,c;

        // General
        cin>>a>>b>>c;


        // Wikivote 
        // cin>>a>>b;
        // c = 1;
        // a--;
        // b--;

        // Gnutella
        // cin>>a>>b;
        // c = 1;

        G[a][b] = G[b][a] = c;
    }
}

void initialize(){
    getGraphAndL();
    getQueueOutDegree();
}

void algo2(int numThreads){
    double start, end, cpu_time_used;

    cout<<"Starting computation\n";

    int i;

    start = omp_get_wtime();

    #pragma omp parallel for shared(Q,L,G,l,numVertices,i) num_threads(numThreads)
    for(i=0;i<numVertices;i++){
        int v = Q[i];
        pruned_dijkstra(v);
    }

    end = omp_get_wtime();
    cpu_time_used = end - start;

    cout<<"Finished computation\n";
    cout<<"Time Elapsed: "<<cpu_time_used<<" sec\n";
}

int main(){
    int numThreads;
    cout<<"Number of threads: ";
    cin>>numThreads;

    
    freopen("./Datasets/test-20-98.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    omp_init_lock(&l);

    initialize();

    algo2(numThreads);

    /*

    // print output
    cout<<"\nL:\n";
    fr(i,0,L.size()){
        fr(j,0,L[i].size()){
            if(L[i][j] == inf) L[i][j] = L[j][i] = Query(i,j);
            // if(L[i][j] != inf) cout<<"L["<<i<<"]["<<j<<"] = "<<L[i][j]<<endl;
            cout<<L[i][j]<<" ";
        }
        cout<<endl;
    }

    */

    return 0;
}

// g++ ParaPLL.cpp -fopenmp