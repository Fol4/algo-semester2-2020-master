#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>
#include <climits>

class Node;

struct Link {
	double val;
	double com;
	Node* node1;
	Node* node2;

	Link(double val, double com, Node* node1, Node* node2) : val(val), com(com), node1(node1), node2(node2) {}
};

class Node {
	uint64_t number;
	std::vector<Link*> links;

public:
	Node(uint64_t number) : number(number) {}

	void AddLink(Link* link) {
		links.push_back(link);
	}

	uint64_t GetNumber() {
		return number;
	}

	const std::vector<Link*>& GetLinks() {
		return links;
	}
};

class Graph {
	std::vector<Node*> nodes;
	std::vector<Link*> links;
	uint64_t size;

	float width(Link* link, double val) {
		return (val - link->com) * link->val;
	}

public:
	Graph(uint64_t size) : size(size) {
		for (uint64_t i = 0; i < size; ++i) {
			nodes.push_back(new Node(i));
		}
	}

	void AddLink(uint64_t in, uint64_t out, double val, double com) {
		Link* NewLink = new Link(val, com, nodes[in], nodes[out]);

		nodes[in]->AddLink(NewLink);
		links.push_back(NewLink);
	}

	bool fordbellman(uint64_t s, double val) {
		std::vector < std::pair<double, double>> dst(size, { INT_MAX, 0 });
		dst[s] = { 0, val };
		int64_t x;
		for (auto i = 0; i < size; ++i) {
			x = -1;
			for (auto link : links) {
				if ((dst[link->node1->GetNumber()].first != INT_MAX) and (dst[link->node2->GetNumber()].first > dst[link->node1->GetNumber()].first +
					dst[link->node1->GetNumber()].second - width(link, dst[link->node1->GetNumber()].second))) {
					dst[link->node2->GetNumber()].first = std::max(0.0-INT_MAX, dst[link->node1->GetNumber()].first +
						dst[link->node1->GetNumber()].second - width(link, dst[link->node1->GetNumber()].second));
					dst[link->node2->GetNumber()].second = width(link, dst[link->node1->GetNumber()].second);
					x = link->node2->GetNumber();
				}
			}
		}

		if (x == -1) {
			return false;
		}

		return true;
	}
};

int main() {
	uint64_t n, m, s;
	double v;
	std::cin >> n >> m >> s >> v;

	Graph graph(n);
	for (auto i = 0; i < m; ++i) {
		uint64_t a, b;
		double val1, com1, val2, com2;
		std::cin >> a >> b >> val1 >> com1 >> val2 >> com2;
		graph.AddLink(a - 1, b - 1, val1, com1);
		graph.AddLink(b - 1, a - 1, val2, com2);
	}

	std::cout << (graph.fordbellman(s - 1, v) ? "YES" : "NO");
}