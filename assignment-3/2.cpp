#include <iostream>
#include <set>
#include <vector>
#include <numeric>
#include <climits>
#include <queue>

enum class Color {
	white, black
};

class Node;

struct Link {
	int64_t val;
	Color color;
	Node* node1;
	Node* node2;

	Link(int64_t val, Node* node1, Node* node2) : val(val), node1(node1), color(Color::white), node2(node2) {}
};

struct cmp {
	bool operator()(Link* lhs, Link* rhs) {
		return lhs->val > rhs->val;
	}
};

class Node {
	uint64_t number;
	Color color;
	std::vector<Link*> links;

public:
	Node(uint64_t number) : number(number), color(Color::white) {}

	void AddLink(Link* link) {
		links.push_back(link);
	}

	uint64_t GetNumber() {
		return number;
	}

	void ChangeColor() {
		color = Color::black;
	}

	Color GetColor() {
		return color;
	}

	const std::vector<Link*>& GetLinks() {
		return links;
	}

};

class Graph {
	std::vector<Node*> nodes;
	std::vector<Link*> links;
	uint64_t size;

public:
	Graph(uint64_t size) : size(size) {
		for (uint64_t i = 0; i < size; ++i) {
			nodes.push_back(new Node(i));
		}
	}

	void AddLink(uint64_t in, uint64_t out, int64_t val) {
		Link* NewLink = new Link(val, nodes[in], nodes[out]);

		nodes[in]->AddLink(NewLink);
		nodes[out]->AddLink(NewLink);
		links.push_back(NewLink);
	}

	uint64_t primFindMST() {
		std::priority_queue<Link*, std::vector<Link*>, cmp> minQ;
		std::vector<uint64_t> sets(size, 0);
		uint64_t sum = 0;
		uint64_t n = 0;
		for (auto link : links) {
			minQ.push(link);
		}

		Link* link = minQ.top();
		minQ.pop();
		sets[link->node1->GetNumber()] = ++n;
		sets[link->node2->GetNumber()] = n;
		sum += link->val;

		while (!minQ.empty()) {
			Link* link = minQ.top();
			minQ.pop();
			if (sets[link->node1->GetNumber()] == sets[link->node2->GetNumber()] and sets[link->node2->GetNumber()] != 0) {
				continue;
			}
			else if (sets[link->node1->GetNumber()] != 0 and sets[link->node2->GetNumber()] != 0) {
				uint64_t newNum = sets[link->node1->GetNumber()];
				for (auto i = 0; i < sets.size(); ++i) {
					if (sets[i] == newNum) {
						sets[i] = sets[link->node2->GetNumber()];
					}
				}
			}
			else {
				if (sets[link->node1->GetNumber()] == 0 and sets[link->node2->GetNumber()] == 0) {
					sets[link->node1->GetNumber()] = ++n;
					sets[link->node2->GetNumber()] = n;
				}
				else {
					sets[link->node1->GetNumber()] == 0 ? sets[link->node1->GetNumber()] = sets[link->node2->GetNumber()] : 
						sets[link->node2->GetNumber()] = sets[link->node1->GetNumber()];
				}
			}

			sum += link->val; 
		}

		return sum;
	}
};

int main() {
	uint64_t n, m;
	std::cin >> n >> m;

	Graph graph(n);
	for (auto i = 0; i < m; ++i) {
		uint64_t node1, node2, val;
		std::cin >> node1 >> node2 >> val;
		graph.AddLink(node1 - 1, node2 - 1, val);
	}

	std::cout << graph.primFindMST();
}