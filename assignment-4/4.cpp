#include <iostream>
#include <vector>
#include <cmath>

class LCA {
	std::vector<std::vector<uint64_t>> tree;
	uint64_t as;
	uint64_t timer;
	std::vector<std::vector<uint64_t>> ancestors;
	std::vector<uint64_t> in;
	std::vector<uint64_t> out;

	void dfs(uint64_t cur, uint64_t parent) {
		in[cur] = ++timer;
		ancestors[cur][0] = parent;

		for (auto i = 1; i < as; ++i) {
			ancestors[cur][i] = ancestors[ancestors[cur][i - 1]][i - 1];
		}

		for (auto i = 0; i < tree[cur].size(); ++i) {
			if (tree[cur][i] != parent) {
				dfs(tree[cur][i], cur);
			}
		}

		out[cur] = timer++;
	}

	bool isA(uint64_t a, uint64_t b) {
		return in[a] <= in[b] and out[b] <= out[a];
	}

public:
	LCA(std::vector<std::vector<uint64_t>>& input) : tree(input), as(static_cast<uint64_t>(ceil(log2(input.size())))), in(input.size()), out(input.size()), ancestors(input.size(), std::vector<uint64_t>(as + 1)), timer(0) {
		dfs(0, 0);
	}

	uint64_t near(uint64_t a, uint64_t b) {
		if (isA(a, b)) {
			return a;
		}
		if (isA(b, a)) {
			return b;
		}

		for (int i = as; i >= 0; --i) {
			if (!isA(ancestors[a][i], b)) {
				a = ancestors[a][i];
			}
		}

		return ancestors[a][0];
	}
};

int main() {
	uint64_t n, m, p;
	std::cin >> n >> m;
	std::vector<std::vector<uint64_t>>input(n);

	for (auto i = 1; i < n; ++i) {
		std::cin >> p;
		input[p].push_back(i);
	}

	LCA lca(input);

	uint64_t a, b, x, y, z;
	std::cin >> a >> b >> x >> y >> z;
	uint64_t sum = 0;
	uint64_t v = 0;
	
	for (auto i = 0; i < m; ++i) {
		v = lca.near((a + v) % n, b);
		a = (a * x + b * y + z) % n;
		b = (b * x + a * y + z) % n;

		sum += v;
	}

	std::cout << sum;
}