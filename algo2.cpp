#include <bits/stdc++.h>
#include <semaphore.h>
#include "algo1.h"

#define inf INT_MAX
#define fr(i,a,b) for(int i=a;i<b;i++)

using namespace std;

typedef pair<int,int> pi;


vector<vector<int>> L, G;
queue<int> Q;

int numVertices;

sem_t q;

vector<pi> getOutDegree(){
    vector<pi> ans;
    fr(i,0,numVertices) ans.push_back(make_pair(1,i));
    return ans;
}

vector<vector<int>> getGraph(){
    vector<vector<int>> G;

    fr(i,0,10){
        vector<int> v;
        fr(j,0,10) v.push_back(i+j);
        G.push_back(v);
    }

    return G;
}

void initialize(){
    G = getGraph();
    numVertices = G.size();
    fr(i,0,numVertices){
        vector<int> v;
        fr(j,0,numVertices) v.push_back(inf);
        L.push_back(v);
    }
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

        L = pruned_dijkstra(G,v,L);
    }
}

int main(){
    sem_init(&q,0,1);
    initialize();

    algo2();

    fr(i,0,L.size()){
        fr(j,0,L[i].size()){
            if(L[i][j] != inf){
                cout<<i<<" "<<j<<" "<<L[i][j]<<endl;
            }
        }
    }

    return 0;
}

// g++ algo2.cpp -lpthread