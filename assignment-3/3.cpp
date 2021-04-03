#include <iostream>
#include <set>
#include <vector>
#include <numeric>
#include <algorithm>
#include <queue>
#include <climits>

struct Link {
	int64_t val;
	uint64_t node1;
	uint64_t node2;

	Link(int64_t val, uint64_t node1, uint64_t node2) : val(val), node1(node1), node2(node2) {}
};

class Graph {
	std::vector<Link*> links;
	std::vector<uint64_t> nodes;
	uint64_t size;

	uint64_t get_leader(int x)
	{
		return (x == nodes[x] ? x : (nodes[x] = get_leader(nodes[x])));
	}

	bool merge(uint64_t x, uint64_t y)
	{
		x = get_leader(x);
		y = get_leader(y);
		if (x == y) return false;
		nodes[x] = y;
		return true;
	}

public:
	Graph(uint64_t size) : size(size) {
		for (auto i = 0; i < size; ++i) {
			nodes.push_back(i);
		}
	}

	void AddLink(uint64_t in, uint64_t out, uint64_t val) {
		Link* NewLink = new Link(val, in, out);

		links.push_back(NewLink);
	}

	uint64_t primFindMST() {
		std::sort(links.begin(), links.end(), [](Link* lhs, Link* rhs) {
			return lhs->val < rhs->val;
			}
		);
		uint64_t sum = 0;

		for (auto& link : links) {
			if (merge(link->node1, link->node2))
				sum += link->val;
		}

		return sum;
	}
};

int main() {
	uint64_t n, m;
	std::cin >> n >> m;
	std::vector<uint64_t> values;

	Graph graph(n);
	for (auto i = 0; i < n; ++i) {
		uint64_t val;
		std::cin >> val;
		values.push_back(val);
	}

	uint64_t first = std::min_element(values.begin(), values.end()) - values.begin();

	for (auto i = 0; i < m; ++i) {
		uint64_t node1, node2, val;
		std::cin >> node1 >> node2 >> val;
		graph.AddLink(node1 - 1, node2 - 1, val);
	}

	for (auto i = 0; i < n; ++i) {
		if (first != i) {
			graph.AddLink(i, first, values[first] + values[i]);
		}
	}

	std::cout << graph.primFindMST();
}