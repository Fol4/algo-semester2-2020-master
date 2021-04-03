#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>


bool bfs(std::vector<std::vector<int64_t>>& edges, std::vector<int64_t>& dist, int64_t s, int64_t t){
	std::fill(dist.begin(), dist.end(), INT_MAX);
	dist[s] = 0;

	std::queue<int64_t> q;
	q.push(s);
	while (!q.empty()){
		int64_t u = q.front();
		q.pop();

		if (u == t)
			return true;

		for (int64_t v : edges[u])
			if (dist[v] == INT_MAX){
				dist[v] = dist[u] + 1;
				q.push(v);
			}
	}

	return false;
}

bool dfs(std::vector<std::vector<int64_t>>& edges, std::vector<int64_t>& dist, int64_t u, int64_t s, int64_t t){
	if (u == t) {
		return 1;
	}

	for (auto v : edges[u]) {
		if (v != s && dist[v] == dist[u] + 1 && dfs(edges, dist, v, s, t)) {
			edges[v].push_back(u);
			edges[u].erase(std::remove(edges[u].begin(), edges[u].end(), v), edges[u].end());

			return 1;
		}
	}

	return 0;
}

int Dinic_algorithm(std::vector<std::vector<int64_t>>& edges, int64_t s, int64_t t){
	int64_t max_flow{ 0 };
	std::vector<int64_t> dist(edges.size());

	while (bfs(edges, dist, s, t)) {
		while (dfs(edges, dist, s, s, t)) {
			++max_flow;
		}
	}

	return max_flow;
}

void preprocessing(std::vector<std::vector<char>>& bridge, std::vector<std::vector<int64_t>>& edges, int64_t n, int64_t m) {
	for (int64_t i = 0; i < n; ++i) {
		for (int64_t j = 0; j < m; ++j) {
			if (bridge[i][j] == '*') {
				if ((i + j) & 1) {
					edges[j + i * m].push_back(n * m + 1);

					if (i > 0 && bridge[i - 1][j] == '*') {
						edges[j + (i - 1) * m].push_back(j + i * m);
					}

					if (j > 0 && bridge[i][j - 1] == '*') {
						edges[j - 1 + i * m].push_back(j + i * m);
					}
				}
				else {
					edges[n * m].push_back(j + i * m);

					if (i > 0 && bridge[i - 1][j] == '*') {
						edges[j + i * m].push_back(j + (i - 1) * m);
					}

					if (j > 0 && bridge[i][j - 1] == '*') {
						edges[j + i * m].push_back(j - 1 + i * m);
					}
				}
			}
		}
	}
}

int main() {
	int64_t n, m, a, b;
	std::cin >> n >> m >> a >> b;
	std::vector<std::vector<char>> bridge(n, std::vector<char>(m));
	int64_t empty{ 0 };


	for (int64_t i = 0; i < n; ++i) {
		for (int64_t j = 0; j < m; ++j) {
			std::cin >> bridge[i][j];

			if (bridge[i][j] == '*')
				++empty;
		}
	}

	if (2 * b <= a) {
		std::cout << b * empty << '\n';
	}
	else if (empty == n * m) {
		std::cout << n * m / 2 * a + ((n * m) % 2) * b;
	}
	else {
		std::vector<std::vector<int64_t>> edges(n * m + 2);
		preprocessing(bridge, edges, n, m);

		int64_t max_flux = Dinic_algorithm(edges, n * m, n * m + 1);
		std::cout << max_flux * a + (empty - 2 * max_flux) * b << '\n';
	}
}