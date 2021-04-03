#include <iostream>
#include <vector>

enum class ColorN {
	white, black
};

class Node;

struct Link {
	int64_t val;
	Node* node1;
	Node* node2;

	Link(int64_t val, Node* node1, Node* node2) : val(val), node1(node1), node2(node2) {}
};

class Node {
	uint64_t number;
	std::vector<ColorN> color;
	std::vector<uint64_t> deep;
	std::vector<Link*> links;

public:
	Node(uint64_t number) : number(number), deep(3, 0), color(3, ColorN::white) {}

	void AddLink(Link* link) {
		links.push_back(link);
	}

	uint64_t GetNumber() {
		return number;
	}

	uint64_t GetDeep(uint64_t number) {
		return deep[number];
	}

	void ChangeDeep(uint64_t newdeep, uint64_t number) {
		deep[number] = newdeep;
	}

	int64_t ChangeSum() {
		return deep[0] + deep[1] + deep[2];
	}

	ColorN GetColor(uint64_t number) {
		return color[number];
	}

	void ChangeColor(ColorN newcolor, uint64_t number) {
		color[number] = newcolor;
	}

	std::vector<Node*> GetLinks(uint64_t number1) {
		std::vector<Node*> nodes;
		for (auto link : links) {
			Node* neigbour = link->node1->GetNumber() != number ? link->node1 : link->node2;
			if (neigbour->GetColor(number1) == ColorN::white) {
				nodes.push_back(neigbour);
			}
		}

		return nodes;
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
		Link* NewLink = new Link(val, nodes[in - 1], nodes[out - 1]);

		nodes[in - 1]->AddLink(NewLink);
		nodes[out - 1]->AddLink(NewLink);
		links.push_back(NewLink);
	}

	void not_dfs(Node* start, uint64_t number) {

		start->ChangeColor(ColorN::black, number);
		start->ChangeDeep(0, number);
		std::vector<Node*> children = start->GetLinks(number);
		uint64_t deep = 0;

		while (!children.empty()) {
			++deep;
			for (auto node : children) {
				node->ChangeDeep(deep, number);
				node->ChangeColor(ColorN::black, number);
			}

			std::vector<Node*> newchildren;
			for (auto node : children) {
				std::vector<Node*> vec = node->GetLinks(number);
				newchildren.insert(newchildren.end(), vec.begin(), vec.end());
			}

			children = newchildren;
		}
	}

	uint64_t path(uint64_t leon, uint64_t matilda, uint64_t milk) {
		not_dfs(nodes[leon - 1], 0);
		not_dfs(nodes[matilda - 1], 1);
		not_dfs(nodes[milk - 1], 2);
		uint64_t Min = nodes[0]->ChangeSum();

		for (auto node : nodes) {
			Min = node->ChangeSum() < Min ? node->ChangeSum() : Min;
		}

		return Min;
	}
};

int main() {
	uint64_t nodes, links, leon, matilda, milk;
	std::cin >> nodes >> links >> leon >> matilda >> milk;
	Graph street(nodes);

	for (uint64_t i = 0; i < links; ++i) {
		uint64_t node1, node2;
		std::cin >> node1 >> node2;

		street.AddLink(node1, node2, 1);
	}

	std::cout << street.path(leon, matilda, milk);
}