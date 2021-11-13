#include <bits/stdc++.h>
#include <semaphore.h>
#include <omp.h>

#define inf INT_MAX
#define fr(i,a,b) for(int i=a;i<b;i++)

using namespace std;

typedef pair<int, int> pi;

vector<vector<int>> L, G;
vector<int> Q;

int numVertices, i=0;

sem_t q, l;

int Query(int s, int t){
    int ans = inf;
    fr(i,0,L.size()) {
        if(L[s][i] != inf && L[t][i] != inf) ans = min(ans, L[s][i]+L[t][i]);
    }
    return ans;
}

void pruned_dijkstra(int v){
    vector<int> D;

    fr(i,0,numVertices) D.push_back(inf);
    D[v] = 0;

    priority_queue<pi,vector<pi>,greater<pi>> Q;
    Q.push(make_pair(0,v));

    while(!Q.empty()){
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

void initialize(){
    vector<tuple<int, int, int>> edges;
    int ne;

    cin>>numVertices;
    cin>>ne;
    cout<<"Num of vertices: "<<numVertices<<endl;
    cout<<"Num of edges: "<<ne<<endl;
    
    vector<int> v;
    vector<pi> outDegree;
    fr(i,0,numVertices) outDegree.push_back(make_pair(0,i));
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
        outDegree[a].first++;
        outDegree[b].first++;
    }
    sort(outDegree.rbegin(), outDegree.rend());
    for(auto x:outDegree) Q.push_back(x.second);
}

void algo2(int numThreads){
    double start, end, cpu_time_used;

    printf("Starting computation...\n");

    start = omp_get_wtime();

    // for(auto x:Q) cout<<x<<" ";
    // cout<<endl;

    #pragma omp parallel shared(Q,L,G,l,q,numVertices,i) num_threads(numThreads)
    {
        #pragma omp while schedule(static)
        while(1){
            sem_wait(&q);
            if(i >= numVertices){
                sem_post(&q);
                break;
            }
            int v = Q[i];
            i++;
            sem_post(&q);

            // To track progress
            // if(i+1%100 == 0) cout<<size<<" vertices done\n";
            
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

    
    freopen("./Datasets/test-20-98.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    sem_init(&q,0,1);
    sem_init(&l,0,1);

    initialize();

    algo2(numThreads);

    // cout<<"\nL:\n";
    fr(i,0,L.size()){
        fr(j,0,L[i].size()){
            if(L[i][j] == inf) L[i][j] = L[j][i] = Query(i,j);
            // if(L[i][j] != inf) cout<<"L["<<i<<"]["<<j<<"] = "<<L[i][j]<<endl;
            cout<<L[i][j]<<" ";
        }
        cout<<endl;
    }

    return 0;
}

// g++ algo3.cpp -fopenmp