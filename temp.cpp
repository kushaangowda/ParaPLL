#include <bits/stdc++.h>
#include <mutex>
#include <condition_variable>
using namespace std;

typedef long long ll;
typedef long double lld;
typedef unsigned long long ull;

#define mod 1000000007
#define send {ios_base::sync_with_stdio(false);}
#define help {cin.tie(NULL);}
#define pii pair<int, int>
#define vi vector<int>
#define fi first
#define se second
#define inf 1e18
#define sp(ans, pre) fixed << setprecision(pre) << y
#define getunique(v) {sort(v.begin(), v.end()); v.erase(unique(v.begin(), v.end()), v.end());}
#define all(x) x.begin(),x.end()
#define pb push_back
#define srt(v) sort(v.begin(), v.end())

class semaphore {
	std::mutex mutex_;
	std::condition_variable condition_;
	unsigned long count_ = 1; // Initialized as locked.

public:
	void release() {
		std::lock_guard<decltype(mutex_)> lock(mutex_);
		++count_;
		condition_.notify_one();
	}

	void acquire() {
		std::unique_lock<decltype(mutex_)> lock(mutex_);
		while (!count_) // Handle spurious wake-ups.
			condition_.wait(lock);
		--count_;
	}

	bool try_acquire() {
		std::lock_guard<decltype(mutex_)> lock(mutex_);
		if (count_) {
			--count_;
			return true;
		}
		return false;
	}
};


int n, m;
vector<vector<pii>> graph;
vector<unordered_map<int, int>> label;
vector<int> degree;
queue<int> vertexQueue;

semaphore queueLocker, queryLocker;

void takeInputAndCalculateDegree()
{
	int x, y, w;
	cin >> n >> m;

	graph = vector<vector<pii>> (n + 1);
	degree = vector<int> (n + 1, 0);

	for (int i = 0; i < m; i++)
	{
		cin >> x >> y >> w;
		graph[x].push_back({y, w});
		graph[y].push_back({x, w});
		degree[x]++, degree[y]++;
	}
}

void createVertexQueue()
{
	vector<pii> dummy(n + 1);
	for (int i = 1; i <= n; i++)
		dummy[i] = {degree[i], i};
	sort(dummy.begin(), dummy.end());
	reverse(dummy.begin(), dummy.end());
	for (auto el : dummy)
	{
		if (el.second != 0)
		{
			vertexQueue.push(el.second);
		}
	}
}

int query(int s, int t)
{
	if (label[s].empty() || label[t].empty())
	{
		return INT_MAX;
	}

	int minm = INT_MAX;

	queryLocker.acquire();
	for (auto it = label[s].begin(); it != label[s].end(); it++)
	{
		int index = it->first, dist1 = it->second, dist2 = 0;
		auto it2 = label[t].find(index);
		if (it2 == label[t].end())
			continue;
		dist2 = it2->second;
		minm = min(minm, dist1 + dist2);
	}
	queueLocker.release();
	return minm;
}

void prunedDijkstra(int k)
{
	vector<int> distance(n + 1, INT_MAX);
	distance[k] = 0;
	priority_queue<pii, vector<pii>, greater<pii>> pq;
	pq.push({distance[k], k});

	while (!pq.empty())
	{
		int u = pq.top().second;
		pq.pop();

		if (query(k, u) <= distance[u])
			continue;

		label[u][k] = distance[u];

		for (auto neigh : graph[u])
		{
			int ver = neigh.first, wt = neigh.second;
			if (distance[u] + wt < distance[ver])
			{
				distance[ver] = distance[u] + wt;
				pq.push({distance[ver], ver});
			}
		}
	}
}

void *paraPLL(void *param)
{
    cout<<vertexQueue.size();
	while (1)
	{
		queueLocker.acquire();
		if (vertexQueue.empty())
		{
			queueLocker.release();
			break;
		}
		int index = vertexQueue.front();
		vertexQueue.pop();
		queueLocker.release();
		prunedDijkstra(index);
	}
    cout<<"Done with vertexQueue\n";
	return NULL;
}

int main()
{
	//for time calculation
#ifndef ONLINE_JUDGE
	auto start = chrono::steady_clock::now();
#endif

// 	send help
// #ifndef ONLINE_JUDGE
// 	freopen("input.txt", "r", stdin);
// 	freopen("output.txt", "w", stdout);
// #endif

	//code starts here
	takeInputAndCalculateDegree();
	createVertexQueue();

	label = vector<unordered_map<int, int>> (n + 1);

	int p = 1;
	pthread_t slave[p];


	for (int i = 0; i < p; i++)
	{
		pthread_create(&slave[i], NULL, paraPLL, NULL);
	}

	for (int i = 0; i < p; i++)
	{
		pthread_join(slave[i], NULL);
	}

    

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			cout << query(i, j) << " ";
		}
		cout << "\n";
	}

#ifndef ONLINE_JUDGE
	auto end = chrono::steady_clock::now();

	auto diff = end - start;

	cout << "\nTime taken: ";
	cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;
#endif
}