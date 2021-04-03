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
	Node* node1;
	Node* node2;

	Link(int64_t val, Node* node1, Node* node2) : val(val), color(ColorL::black), node1(node1), node2(node2) {}
};

class Node {
	uint64_t number;
	uint64_t distance;
	ColorN color;
	std::vector<Link*> links;

public:
	Node(uint64_t number) : number(number), color(ColorN::white) {}

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
};

class Graph {
	std::vector<Node*> nodes;
	std::vector<Link*> links;
	uint64_t size;
	uint64_t maximum = 0;

public:
	Graph(uint64_t size) : size(size) {
		for (uint64_t i = 0; i < size; ++i) {
			nodes.push_back(new Node(i));
		}
	}

	void AddLink(uint64_t in, uint64_t out, int64_t val) {
		Link* NewLink = new Link(val, nodes[in], nodes[out]);

		nodes[in]->AddLink(NewLink);
		maximum += val;
		links.push_back(NewLink);
	}

	void not_dfs(Node* start, uint64_t number) {

		start->ChangeColor(ColorN::gray);
		std::vector<Node*> path;
		path.push_back(start);

		for (auto i = 0; !path.empty() and i < number; ++i) {
			std::vector<Node*> newpath;
			for (auto node : path) {
				std::vector<Link*> neighbours = node->GetLinks();
				for (auto link : neighbours) {
					if (link->node2->GetColor() != ColorN::gray) {
						newpath.push_back(link->node2);
						link->node2->ChangeColor(ColorN::gray);
					}
				}
			}

			path = newpath;
		}
	}

	int64_t dijkstra(uint64_t number, uint64_t number2, uint64_t k) {

		not_dfs(nodes[number], k);

		if (number == number2)
			return 0;

		nodes[number]->ChangeDistance(0);
		std::priority_queue < std::pair<int64_t, int64_t>, std::vector<std::pair<int64_t, int64_t>>, std::greater<std::pair<int64_t, int64_t>>> local;
		local.push({ 0, number });
		nodes[number]->ChangeColor(ColorN::black);

		while (!local.empty()) {
			int64_t now = local.top().second;
			int64_t dist = local.top().first;
			local.pop();

			if (now == number2) {
				return dist;
			}

			std::vector<Link*> neighbours = nodes[now]->GetLinks();

			for (auto link : neighbours) {
				if (link->node2->GetColor() == ColorN::gray) {
					nodes[now]->ChangeColor(ColorN::black);
					local.push({ link->val + dist, link->node2->GetNumber() });
				}
			}
		}

		return -1;
	}
};

int main() {
	uint64_t n, m, k, s, f;
	std::cin >> n >> m >> k >> s >> f;

	Graph graph(n);

	for (auto i = 0; i < m; ++i) {
		uint64_t node1, node2, val;
		std::cin >> node1 >> node2 >> val;

		graph.AddLink(node1 - 1, node2 - 1, val);
	}

	
	std::cout << graph.dijkstra(s - 1, f - 1, k);
}