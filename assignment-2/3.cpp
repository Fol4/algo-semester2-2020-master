#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>
#include <queue>
#include <climits>
//
//enum class ColorN {
//	white, gray, black
//};
//
//enum class ColorL {
//	green, blue, red, black
//};
//
//class Node;
//
//struct Link {
//	float val;
//	ColorL color;
//	Node* node1;
//	Node* node2;
//
//	Link(float val, Node* node1, Node* node2) : val(val), color(ColorL::black), node1(node1), node2(node2) {}
//};
//
//class Node {
//	uint64_t number;
//	ColorN color;
//	std::vector<Link*> links;
//
//public:
//	Node(uint64_t number) : number(number), color(ColorN::white) {}
//
//	void AddLink(Link* link) {
//		links.push_back(link);
//	}
//
//	void ChangeColor(ColorN colorl) {
//		color = colorl;
//	}
//
//	uint64_t GetNumber() {
//		return number;
//	}
//
//	ColorN GetColor() {
//		return color;
//	}
//
//	const std::vector<Link*>& GetLinks() {
//		return links;
//	}
//};
//
//class Graph {
//	std::vector<Node*> nodes;
//	std::vector<Link*> links;
//	uint64_t size;
//	uint64_t maximum = 0;
//
//public:
//	Graph(uint64_t size) : size(size) {
//		for (uint64_t i = 0; i < size; ++i) {
//			nodes.push_back(new Node(i));
//		}
//	}
//
//	void AddLink(uint64_t in, uint64_t out, float val) {
//		Link* NewLink = new Link(val, nodes[in], nodes[out]);
//
//		nodes[in]->AddLink(NewLink);
//		maximum += val;
//		links.push_back(NewLink);
//	}
//
//	float dijkstra(uint64_t number, uint64_t number2) {
//
//		std::priority_queue < std::pair<float, int64_t>, std::vector<std::pair<float, int64_t>>, std::greater<std::pair<float, int64_t>>> local;
//		local.push({ 0, number });
//
//		while (!local.empty()) {
//			int64_t now = local.top().second;
//			float dist = local.top().first;
//			local.pop();
//
//			nodes[now]->ChangeColor(ColorN::black);
//			if (now == number2) {
//				return dist;
//			}
//
//			std::vector<Link*> neighbours = nodes[now]->GetLinks();
//
//			for (auto link : neighbours) {
//				if (link->node2->GetColor() != ColorN::black) {
//					local.push({ (1.0 - dist) * link->val + dist, link->node2->GetNumber() });
//				}
//			}
//		}
//	}
//};


enum class ColorN {
	white, grey, black
};

enum class ColorL {
	green, blue, red, black
};

class Node;

struct Link {
	float val;
	ColorL color;
	Node* node1;
	Node* node2;

	Link(float val, Node* node1, Node* node2) : val(val), color(ColorL::black), node1(node1), node2(node2) {}
};

class Node {
	uint64_t number;
	float distance;
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

	float GetDistance() {
		return distance;
	}

	void ChangeDistance(float newDist) {
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

	void AddLink(uint64_t in, uint64_t out, float val) {
		Link* NewLink = new Link(val, nodes[in], nodes[out]);

		nodes[in]->AddLink(NewLink);
		links.push_back(NewLink);

		Link* NewLink2 = new Link(val, nodes[out], nodes[in]);

		nodes[out]->AddLink(NewLink2);
		links.push_back(NewLink2);
	}

	float dijkstra(uint64_t number, uint64_t number2) {

		if (number == number2)
			return 0;

		for (auto node : nodes) {
			node->ChangeDistance(INT_MAX);
		}

		nodes[number]->ChangeDistance(0);
		std::priority_queue < std::pair<float, int64_t>, std::vector<std::pair<float, int64_t>>, std::greater<std::pair<float, int64_t>>> local;
		local.push({ nodes[number]->GetDistance(), number });

		while (!local.empty()) {
			int64_t now = local.top().second;
			float dist = local.top().first;
			local.pop();

			std::vector<Link*> neighbours = nodes[now]->GetLinks();

			for (auto link : neighbours) {
				if ((1 - dist) * link->val + dist < link->node2->GetDistance()) {
					link->node2->ChangeDistance((1 - dist) * link->val + dist);
					local.push({ (1 - dist)*link->val + dist, link->node2->GetNumber() });
				}
			}
		}

		return nodes[number2]->GetDistance();
	}
};

int main() {
	uint64_t n, m, s, f;
	std::cin >> n >> m >> s >> f;

	Graph graph(n);

	for (auto i = 0; i < m; ++i) {
		uint64_t node1, node2;
		float val;
		std::cin >> node1 >> node2 >> val;

		graph.AddLink(node1 - 1, node2 - 1, val / 100);
	}


	std::cout << graph.dijkstra(s - 1, f - 1);
}