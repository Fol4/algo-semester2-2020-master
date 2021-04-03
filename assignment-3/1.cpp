#include <set>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <numeric>
#include <climits>

enum class Color{
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

class Node {
	uint64_t number;
	Color color;
	std::vector<Link*> links;

public:
	Node(uint64_t number) : number(number), color(Color::white){}

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
		std::vector<uint64_t> dst(size, INT_MAX);
		std::set<std::pair<uint64_t, Node*>> minQ;
		dst[0] = 0;
		minQ.insert({ dst[0], nodes[0] });

		for(auto i = 0; i < size; ++i) {
			Node* cur = minQ.begin()->second;
			minQ.erase(minQ.begin());

			if (cur->GetColor() == Color::black) {
				continue;
			}

			cur->ChangeColor();

			for (auto link : cur->GetLinks()) {
				auto out = (link->node1 == cur ? link->node2 : link->node1);
				if (dst[out->GetNumber()] > link->val and link->color != Color::black) {
					minQ.erase({ dst[out->GetNumber()], out });
					dst[out->GetNumber()] = link->val;
					link->color = Color::black;
					minQ.insert({ dst[out->GetNumber()], out });
				}
			}
		}

		return std::accumulate(dst.begin(), dst.end(), 0);
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