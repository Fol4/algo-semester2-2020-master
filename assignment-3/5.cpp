#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>
#include <queue>
#include <climits>

enum class ColorN {
	white, gray, black
};

enum class ColorL {
	green, blue, red, black
};

class Node;

struct Link {
	int64_t val;
	ColorL color;
	bool reversed;
	Node* node1;
	Node* node2;

	Link(int64_t val, Node* node1, Node* node2) : val(val), color(ColorL::black), node1(node1), node2(node2), reversed(false) {}
};

class Node {
	uint64_t number;
	uint64_t distance;
	ColorN color;
	std::vector<Link*> links;

public:
	Node(uint64_t number) : number(number), color(ColorN::white), distance(INT_MAX) {}

	void AddLink(Link* link) {
		links.push_back(link);
	}

	void ChangeColor(ColorN colorl) {
		color = colorl;
	}

	uint64_t GetNumber() {
		return number;
	}

	ColorN GetColor() {
		return color;
	}

	const std::vector<Link*>& GetLinks() {
		return links;
	}

	uint64_t GetDistance() {
		return distance;
	}

	void ChangeDistance(uint64_t newDist) {
		distance = newDist;
	}

	void ChangeLink(Node* node, int64_t dst1, int64_t dst2) {
		for (auto link : links) {
			if (link->node2 == node) {
				link->val -= dst2 - dst1;
				break;
			}
		}
	}

	void ChangeLinkColor(Node* node) {
		for (auto link : links) {
			if (link->node2 == node) {
				link->color = ColorL::red;
				return;
			}
		}
	}
};

class Graph {
	uint64_t size;
	std::vector<Node*> nodes;

	int64_t search_bridge(int64_t start, int64_t end) {
		if (start == end) {
			return 1;
		}

		nodes[start]->ChangeColor(ColorN::gray);

		for (auto link : nodes[start]->GetLinks()){
			auto node2 = link->node1 == nodes[start] ? link->node2 : link->node1;

			if ((node2 == link->node1) != link->reversed)
				continue;

			if (node2->GetColor() == ColorN::white and search_bridge(node2->GetNumber(), end)) {
				link->reversed = !link->reversed;
				return 1;
			}
		}

		return -1;
	}

	int64_t search_path(int64_t start, int64_t end, std::vector<int64_t>& path) {
		if (start == end) {
			path.push_back(start);
			return 1;
		}

		nodes[start]->ChangeColor(ColorN::gray);

		for (auto link : nodes[start]->GetLinks()) {
			auto node2 = link->node1 == nodes[start] ? link->node2 : link->node1;

			if (node2 == link->node2 and link->reversed and node2->GetColor() == ColorN::white and search_path(node2->GetNumber(), end, path)) {

				link->reversed = false;
				path.push_back(start);
				return 1;
			}
		}

		return -1;
	}

	void clear() {
		for (auto node : nodes) {
			node->ChangeColor(ColorN::white);
		}
	}

public:
	Graph(uint64_t size) : size(size) {
		for (uint64_t i = 0; i < size; ++i) {
			nodes.push_back(new Node(i));
		}
	}

	void AddLink(uint64_t in, uint64_t out, int64_t val) {
		if (in != out) {
			Link* NewLink = new Link(val, nodes[in], nodes[out]);

			nodes[in]->AddLink(NewLink);
			nodes[out]->AddLink(NewLink);
		}
	}

	void answer(int64_t start, int64_t end) {
		if (search_bridge(start, end) == -1) {
			std::cout << "NO";
			return;
		}

		clear();

		if (search_bridge(start, end) == -1) {
			std::cout << "NO";
			return;
		}

		std::vector<int64_t> path1;
		std::vector<int64_t> path2;
		int64_t i;

		
		for (i = 0; i < 1; ++i) {

			if (search_path(start, end, path1))
				break;

			clear();
		}

		if (path1.empty()) {

			std::cout << "NO";
			return;
		}
		
		clear();

		for (; i < size; ++i) {

			if (search_path(start, end, path2))
				break;

			clear();
		}

		if (path2.empty()) {

			std::cout << "NO";
			return;
		}

		std::cout << "YES" << std::endl;

		for (i = path1.size() - 1; i >= 0; --i) {
			std::cout << path1[i] + 1 << ' ';
		}

		std::cout << std::endl;

		for (i = path2.size() - 1; i >= 0; --i) {
			std::cout << path2[i] + 1 << ' ';
		}
	}
};

int main() {
	int64_t n, m, s, t;
	std::cin >> n >> m >> s >> t;

	Graph tree(n);

	for (auto i = 0; i < m; ++i) {
		int64_t in, out;
		std::cin >> in >> out;
		tree.AddLink(in - 1, out - 1, 1);
	}

	tree.answer(s - 1, t - 1);
}