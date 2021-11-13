#include <bits/stdc++.h>
#include <semaphore.h>
#include <omp.h>

#define inf INT_MAX
#define fr(i,a,b) for(int i=a;i<b;i++)

using namespace std;

typedef pair<int, int> pi;

vector<vector<int>> L, G;
queue<int> Q;

int numVertices;

sem_t q, l;

int Query(int s, int t){
    int ans = inf;
    fr(i,0,L.size()) {
        if(L[s][i] != inf && L[t][i] != inf) ans = min(ans, L[s][i]+L[t][i]);
    }
    return ans;
}

void pruned_dijkstra(int v){
    int numVertices = G.size();
    vector<int> D;

    // cout<<endl;
    // cout<<"Vertex "<<v<<endl;

    fr(i,0,numVertices) D.push_back(inf);
    D[v] = 0;

    priority_queue<pi,vector<pi>,greater<pi>> Q;
    Q.push(make_pair(0,v));

    while(!Q.empty()){
        // cout<<Q.top().first<<" "<<Q.top().second<<endl;
        int u = Q.top().second;
        Q.pop();

        sem_wait(&l);
        if(Query(v,u) <= D[u]){
            sem_post(&l);
            continue;
        }
        L[u][v] = D[u];
        sem_post(&l);

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
    for(auto x:ans) Q.push(x.second);
}

void getGraphAndL(){
    vector<tuple<int, int, int>> edges;
    int ne;

    cout<<"Num of vertices: ";
    cin>>numVertices;
    cout<<"Num of edges: ";
    cin>>ne;
    
    vector<int> v;
    fr(i,0,numVertices) v.push_back(inf);
    fr(i,0,numVertices) G.push_back(v);
    fr(i,0,numVertices) L.push_back(v);

    fr(i,0,ne){
        int a,b,c;
        cin>>a>>b>>c;
        G[a][b] = G[b][a] = c;
    }
}

void initialize(){
    getGraphAndL();
    getQueueOutDegree();
}

void algo2(int numThreads){
    double start, end, cpu_time_used;

    start = omp_get_wtime();

    #pragma omp parallel shared(Q,L,G,l,q,numVertices) num_threads(numThreads)
    {
        #pragma omp while schedule(static)
        while(!Q.empty()){
            sem_wait(&q);
            int v = Q.front();
            Q.pop();
            sem_post(&q);

            pruned_dijkstra(v);
        }
    }

    end = omp_get_wtime();
    cpu_time_used = end - start;

    printf("Finished computation\n");
    printf("Time Elapsed: %f sec\n", cpu_time_used);
}

int main(){
    int numThreads;
    cout<<"Number of threads: ";
    cin>>numThreads;

    sem_init(&q,0,1);
    sem_init(&l,0,1);

    initialize();

    algo2(numThreads);

    cout<<"\nL:\n";
    fr(i,0,L.size()){
        fr(j,0,L[i].size()){
            if(L[i][j] == inf) L[i][j] = L[j][i] = Query(i,j);
            // if(L[i][j] != inf) cout<<"L["<<i<<"]["<<j<<"] = "<<L[i][j]<<endl;
            cout<<L[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;

    return 0;
}

// g++ algo2.cpp -fopenmp